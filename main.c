#include <LPC21xx.h>
#include "defines.h"
#include "delay.h"
#include "lcd_defines.h"
#include "Interruptenable.h"
#include "kpm.h"
#include "LCD_INIT.h"
#include "RTC.h"
#include "adc.h"

/* ---------- Tiny setters ---------- */
void SetHour(unsigned int h)   { HOUR  = h; }
void SetMin(unsigned int m)    { MIN   = m; }
void SetSec(unsigned int s)    { SEC   = s; }
void SetDOW(unsigned int d)    { DOW   = d; }
void SetDOM(unsigned int d)    { DOM   = d; }
void SetMonth(unsigned int m)  { MONTH = m; }
void SetYear(unsigned int y)   { YEAR  = y; }

int defaultflag = 0;
int Interrupt_Triggered = 0;

/* day strings */
static const char* day_names[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

static const char* get_day_string(int day)
{
    if(day >= 0 && day <= 6) return day_names[day];
    return "Err";
}

/* ---- Edit RTC menu ---- */
void edit_time(void)
{
    int menu_choice;
    char menu_key;       // renamed from 'key' to avoid conflict

    cmdLCD(CLEAR_LCD);
    defaultflag = 0;

    cmdLCD(GOTO_LINE1_POS0);
    strLCD("Menu");
    cmdLCD(GOTO_LINE2_POS0);
    strLCD("1.Edit RTC 2.Exit");

    menu_choice = KeyScan();
    while (ColScan() == 0);
    delay_ms(200);

    if (menu_choice != 1) {
        Interrupt_Triggered = 0;
        return;
    }

    cmdLCD(CLEAR_LCD);
    strLCD("Editing RTC...");
    delay_ms(600);

start:
    cmdLCD(CLEAR_LCD);
    strLCD("1H 2M 3S 4D");
    cmdLCD(GOTO_LINE2_POS0);
    strLCD("5Dt 6Mo 7Yr");

    menu_key = KeyScan();
    while (ColScan() == 0);
    delay_ms(180);

    switch(menu_key) {
        case 1: ValidateAndSet("Set HOUR:",  0, 23, SetHour);  break;
        case 2: ValidateAndSet("Set MIN:",   0, 59, SetMin);   break;
        case 3: ValidateAndSet("Set SEC:",   0, 59, SetSec);   break;
        case 4: ValidateAndSet("Set DAY(0-6):", 0, 6, SetDOW); break;
        case 5: ValidateAndSet("Set DATE:",  1, 31, SetDOM);   break;
        case 6: ValidateAndSet("Set MONTH:", 1, 12, SetMonth); break;
        case 7: ValidateAndSet("Set YEAR:",  2000, 2099, SetYear); break;
        default:
            defaultflag = 1; 
            break;
    }

    if(defaultflag == 1) defaultflag = 0;
    else goto start;

    cmdLCD(CLEAR_LCD);
    strLCD("RTC Updated!");
    delay_ms(600);
    Interrupt_Triggered = 0;
}

/* ---- Main ---- */
int main(void)
{
    unsigned int ldr_digital_val = 0;
    float ldr_voltage = 0.0f;

    Init_KPM();
    INIT_LCD();

    cmdLCD(DSP_ON_CUR_BLINK);
    cmdLCD(CLEAR_LCD);
    cmdLCD(GOTO_LINE1_POS0);
    strLCD("Hello");
    delay_s(2);

    enable_eint0();
    rtc_init();
    rtc_Set_Pre();

    delay_ms(400);
    Init_ADC();

    IODIR0 |= (0xFFu << 18);  // LEDs P0.18-P0.25 output

    while(1)
    {
        /* Display Time on Line1 */
        cmdLCD(GOTO_LINE1_POS0);
        charLCD((HOUR/10) + '0');
        charLCD((HOUR%10) + '0');
        charLCD(':');
        charLCD((MIN/10) + '0');
        charLCD((MIN%10) + '0');
        charLCD(':');
        charLCD((SEC/10) + '0');
        charLCD((SEC%10) + '0');

        /* Display Date + DOW on Line2 */
        cmdLCD(GOTO_LINE2_POS0);
        charLCD((DOM/10) + '0');
        charLCD((DOM%10) + '0');
        charLCD('/');
        charLCD((MONTH/10) + '0');
        charLCD((MONTH%10) + '0');
        charLCD('/');
        charLCD(((YEAR % 100)/10) + '0');
        charLCD(((YEAR % 100)%10) + '0');
        charLCD(' ');
        strLCD((char*)get_day_string(DOW));  // CHG: cast to char* for Keil

        /* Read LDR */
        Read_ADC(0, &ldr_digital_val, &ldr_voltage);

        /* Night logic: 18:0005:59 LEDs ON */
        if(HOUR >= 18 || HOUR < 6) {
            int led_count = (int)((1023 - (int)ldr_digital_val) * 8 / 1023);
            if(led_count < 0) led_count = 0;
            if(led_count > 8) led_count = 8;

            IOCLR0 = (0xFFu << 18);
            if(led_count > 0) IOSET0 = (((1u << led_count) - 1u) << 18);
        } else {
            IOCLR0 = (0xFFu << 18); // daytime LEDs OFF
        }

        delay_ms(200);

        if(Interrupt_Triggered == 1) {
            edit_time();
            cmdLCD(CLEAR_LCD); // refresh LCD after editing
        }
    }
}
