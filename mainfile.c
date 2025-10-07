#include <LPC21xx.h>
#include "defines.h"
#include "delay.h"
#include "lcd_defines.h"
#include "Interruptenable.h"
#include "kpm.h"
#include "LCD_INIT.h"
#include "RTC.h"
#include "adc.h"
#include "types.h"

#define LEDS (0xFF000000)  // P1.24<96>P1.31

void SetHour(u32 h)  { HOUR = h; }
void SetMin(u32 m)   { MIN = m; }
void SetSec(u32 s)   { SEC = s; }
void SetDOW(u32 d)   { DOW = d; }
void SetDOM(u32 d)   { DOM = d; }
void SetMonth(u32 m) { MONTH = m; }
void SetYear(u32 y)  { YEAR = y; }

int defaultflag=0;
int Interrupt_Triggered=0;

char* get_day_string(int day)
{
    static char* day_names[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    if (day >= 0 && day <= 6) {
        return day_names[day];
    } else {
        return "Err";
    }
}

void edit_time(void) {
    char menu_choice;
    char key;

    cmdLCD(CLEAR_LCD);
    defaultflag = 0;

    cmdLCD(GOTO_LINE1_POS0);
    strLCD("1.Edit RTC");
    cmdLCD(GOTO_LINE2_POS0);
    strLCD("2.Exit");

    menu_choice = KeyScan();
    while (ColScan() == 0);
    delay_ms(200);

    if (menu_choice != '1') {
        Interrupt_Triggered = 0;
        return;
    }

    cmdLCD(CLEAR_LCD);
    strLCD("Editing RTC...");
                cmdLCD(CLEAR_LCD);
    delay_ms(1000);

start:
    cmdLCD(CLEAR_LCD);
    strLCD("1H 2M 3S 4D");
    cmdLCD(GOTO_LINE2_POS0);
    strLCD("5Dt 6Mo 7Yr 8Ex");

    key = KeyScan();
    while (ColScan() == 0);
    delay_ms(200);

    switch (key) {
        case '1':
            ValidateAndSet("Set HOUR:", 0, 23, SetHour);
            break;
        case '2':
            ValidateAndSet("Set MIN:", 0, 59, SetMin);
            break;
        case '3':
            ValidateAndSet("Set SEC:", 0, 59, SetSec);
            break;
        case '4':
            ValidateAndSet("Set DAY :", 0, 6, SetDOW);
            break;
        case '5':
            ValidateAndSet("Set DATE:", 1, 31, SetDOM);
            break;
        case '6':
            ValidateAndSet("Set MONTH:", 1, 12, SetMonth);
            break;
        case '7':
            ValidateAndSet("Set YEAR:", 2000, 2099, SetYear);
            break;
        case '8':cmdLCD(0x01);
            return; // Exit edit mode
        default:
            defaultflag = 1;
            break;
    }

    if (defaultflag == 1) {
        defaultflag = 0;
    } else {
        goto start;
    }

    cmdLCD(CLEAR_LCD);
    strLCD("RTC Updated!");
    delay_ms(1000);
    Interrupt_Triggered = 0;
}

int main()
{
    unsigned int ldr_digital_val;
    float ldr_voltage;

    Init_KPM();
    INIT_LCD();
    strLCD("Hello");
    delay_ms(1000);

    enable_eint0();
    rtc_init();
    rtc_Set_Pre();
    Init_ADC();

    IODIR1 |= LEDS;   // LEDs as output
    IOCLR1 = LEDS;  // LEDs OFF initially

    while(1)
    {
        // Show Time
        cmdLCD(GOTO_LINE1_POS0);
        charLCD(HOUR/10+'0');
        charLCD(HOUR%10+'0');
        charLCD(':');
        charLCD(MIN/10+'0');
        charLCD(MIN%10+'0');
        charLCD(':');
        charLCD(SEC/10+'0');
        charLCD(SEC%10+'0');

        // Show Date
        cmdLCD(GOTO_LINE2_POS0);
        charLCD(DOM/10 + '0');
        charLCD(DOM%10 + '0');
        charLCD('/');
        charLCD(MONTH/10 + '0');
        charLCD(MONTH%10 + '0');
        charLCD('/');
        charLCD((YEAR % 100)/10 + '0');
        charLCD((YEAR % 100)%10 + '0');
        charLCD(' ');
        strLCD(get_day_string(DOW));

        // Read LDR
        Read_ADC(0, &ldr_digital_val, &ldr_voltage);
				
				cmdLCD(GOTO_LINE1_POS0+13);
        f32LCD(ldr_voltage,2);

        // Control LEDs based on time + LDR
        if (HOUR >= 18 || HOUR < 6)
        {
            unsigned int led_count = 8 - (ldr_digital_val * 8 / 1024);
            IOCLR1 = LEDS;
            IOSET1 = ((1U<<led_count)-1)<< 24;  ;
        }
        else {
            IOCLR1 = LEDS;
        }

        delay_ms(1000);

        // Handle RTC Edit interrupt
        if(Interrupt_Triggered==1)
        {
            edit_time();
            cmdLCD(CLEAR_LCD);
        }
    }
}
