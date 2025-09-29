#include<LPC21xx.h>
#include "delay.h"

#define LCD_DATA 8 //p0.8-p0.15
#define LCD_RS 17
#define LCD_EN 18

#define CLEAR_LCD 0x01
#define RET_CUR_HOME 0x02
#define SHIFT_CUR_RIGHT 0x06
#define SHIFT_CUR_LEFT 0x07
#define DISP_OFF 0x08
#define DISP_ON_CUR_OFF 0x0c
#define DISP_ON_CUR_ON 0x0e
#define DISP_ON_CUR_BLINK 0x0f
#define SHIFT_DISP_RIGHT 0x10
#define SHIFT_DISP_LEFT 0x14
#define MODE_8BIT_LINE1 0x30
#define MODE_8BIT_LINE2 0x38
#define MODE_4BIT_LINE1 0x20
#define MODE_4BIT_LINE2 0x28
#define GOTO_LINE1_POS0 0x80
#define GOTO_LINE2_POS0 0xC0
#define GOTO_CGRAM 0x40
#define WBYTE(WORD,STARTBIT,BYTE) (WORD=((WORD &~(0xffu<<STARTBIT))| (BYTE<<STARTBIT)))
#define SSETBIT(WORD,STARTBIT) (WORD=1<<STARTBIT)
#define SCLRBIT(WORD,STARTBIT) (WORD=1<<STARTBIT)
void WRITELCD(unsigned char byte)
{
	WBYTE(IOPIN0,LCD_DATA,byte);
	SSETBIT(IOSET0,LCD_EN);
	delay_us(1);
	SCLRBIT(IOCLR0,LCD_EN);
	delay_ms(2);
}
void cmdLCD(unsigned char cmd)
{
	SCLRBIT(IOCLR0,LCD_RS);
	WRITELCD(cmd);
}
void charLCD(unsigned char byte)
{
	SSETBIT(IOSET0,LCD_RS);
	WRITELCD(byte);
}

void INIT_LCD(void)
{
	IODIR0 |= (((unsigned)0xff<<LCD_DATA))|(1<<LCD_RS)|(1<<LCD_EN);
	delay_ms(15);
	cmdLCD(0x30);
	delay_ms(5);
	cmdLCD(0x30);
	delay_us(100);
	cmdLCD(0x30);
	cmdLCD(MODE_8BIT_LINE2);
	cmdLCD(DISP_ON_CUR_ON);
	cmdLCD(CLEAR_LCD);
	cmdLCD(SHIFT_CUR_RIGHT);
}

void strLCD(char *s)
{
	while(*s)
		charLCD(*s++);
}

void u32LCD(unsigned int num)
{
	unsigned char str[10];
	int i=0;
	if(num==0)
		charLCD('0');
	else
	{		
		while(num)
		{
			str[i++]=(num%10)+48;
			num/=10;
		}
		for(--i;i>=0;i--)
		{
			charLCD(str[i]);
		}
	}
}

void s32LCD(int num)
{
	if(num<0)
	{
		charLCD('-');
		num=-num;
	}
	u32LCD(num);
}

void hexLCD(unsigned int num)
{
	int i=0;
	char a[100],t;
	if(num==0)
	{
		charLCD('0');
	}
	while(num)
	{
		t=num%16;
		(t>9)?(t=(t-10)+'A'):(t=t+48);
		a[i++]=t;
		num/=16;
	}
	for(--i;i>=0;i--)
	{
		charLCD(a[i]);
	}
}

void octLCD(unsigned int num)
{
	int i=0;
	char a[100],t;
	if(num==0)
		charLCD('0');
	while(num)
	{
		t=num%8;
		a[i++]=(t+48);
		num/=8;
	}
	for(--i;i>=0;i--)
	{
		charLCD(a[i]);
	}
}

void binLCD(unsigned int num,int bits)
{
	for(;bits>=0;bits--)
	{
		charLCD(((num>>bits)&1)+48);
	}
}
