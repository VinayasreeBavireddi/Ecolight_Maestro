//kpm.c
#include<LPC21XX.h>
#include"defines.h"
#include"kpm_defines.h"
#include "delay.h"
#include "LCD_INIT.h"
unsigned char KPMLUT[4][4]={{'7','8','9','/'},
                            {'4','5','6','X'},
                            {'1','2','3','-'},
                            {'C','0','=','+'}};
void Init_KPM(void)
{
        //cfg p1.16 t0 p1.19 as gpio out for rowlines
        WRITENIBBLE(IODIR1,ROW0,15);
        //default all column lines are high input
}
unsigned int ColScan(void)
{
        unsigned int status;
        status=(READNIBBLE(IOPIN1,COL0)<15)? 0:1;
        return status;
}
unsigned int RowCheck(void)
{
        unsigned int r;
        for(r=0;r<=3;r++)
        {
                //ground one row at a time,keeping others high row's
                //starting with 0th row
                WRITENIBBLE(IOPIN1,ROW0,~(1<<r));
                //chexk if any key is pressed in that row
                if(ColScan()==0)
                {
                        break;
                }
        }
        //reintialize all rows to ground
        WRITENIBBLE(IOPIN1,ROW0,0);
        return r;
}
unsigned int ColCheck(void)
{
        unsigned int c;
        for(c=0;c<3;c++)
        {
                if(READBIT(IOPIN1,(COL0+c))==0)
                {
                        break;
                }
        }
        return c;
}
unsigned int KeyScan(void)
{
        unsigned int r,c,key;
        //write for any keypress
				delay_ms(500);
        while(ColScan());
        //if any key pressed identify row in which key was pressed
        r=RowCheck();
        //identify col in which key is pressed
        c=ColCheck();
        //extract key value from lookup table using row & col indices
        key=KPMLUT[r][c];
        //return value
        return  key;
}
void ReadNum(unsigned int *num, char *key)
{
    int digits=0;
		char lastDigit=0;
    *num=0;
    

    while(1)
    {
        *key = KeyScan();            
        while(ColScan()==0);       
        delay_ms(200);

        if(*key>='0' && *key<='9')   // digit
        {
            *num=(*num*10)+(*key-'0');
            digits++;
            charLCD(*key);
            lastDigit=*key;   // store last digit key
        }
        else if(*key=='+')   // backspace
        {
            if(digits>0)
            {
                *num = *num / 10;
                digits--;
                cmdLCD(0x10);
                charLCD(' ');
                cmdLCD(0x10);
            }
        }
        else if(*key=='C')   // clear all
        {
            *num=0;
            digits=0;
            cmdLCD(0x01);
            strLCD("Cleared");  
            delay_ms(500);
            cmdLCD(0x01);
        }
        else if(*key == '=')   // enter key
        {
            *key = lastDigit;  // return the last digit, not '='
            break;
        }
        else   
        {
            break;
        }
    }
}
