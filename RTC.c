#include <LPC21xx.h>
#include "RTC_defines.h"
#include "kpm.h"
#include "LCD_INIT.h"
#include "types.h"
#include "delay.h"
#include "lcd_defines.h"
void rtc_init()
{
	SEC=50; //Initialized seconds
  MIN=30; //Initialized minutes
  HOUR=06;//Initialized hour
}
void rtc_Set_Pre()
{
	PREINT=PREINT_VAL;
  PREFRAC=PREFRAC_VAL;
	AMR=0xFF;
	CCR=0x01;
}

void ValidateAndSet(char *msg, u32 minVal, u32 maxVal, void (*setFunc)(u32))
{
    unsigned int val;
    char key;
    while(1)
    {
        cmdLCD(0x01);
        strLCD(msg);
        ReadNum((int*)&val, &key);

        if(val >= minVal && val <= maxVal)
        {
            setFunc(val);
            break;
        }
        else
        {
            cmdLCD(0x01);
            strLCD("Invalid Range!");
            delay_ms(500);
        }
    }
}

