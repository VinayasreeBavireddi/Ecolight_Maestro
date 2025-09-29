//adc.c
#include<LPC21XX.h>
#include"delay.h"
#include"pin_connect_block.h"
#include "adc_defines.h"
void Init_ADC(void)
{
        //cfg p0.27 as a input pin
        CfgPortPinFunc(0,27,1);
        //cfg ADCR
        ADCR=(CLKDIV<<CLKDIV_BITS)|(1<<PDN_BIT);
}
void Read_ADC(unsigned int chNo,unsigned int *adcDVal,float *eAR)
{
        //MASK any prev channel sel
        ADCR&=0XFFFFFF00;
        //write channel for analog_read,
        //&start start adc conversation wrt that channel
        ADCR|=(1<<chNo)|(1<<ADC_CONV_START_BIT);
        delay_us(3);
        //wait until conversion completion status
        while(((ADDR>>DONE_BIT)&1)==0);
        //stop adc convert wrt that channel
        ADCR&=~(1<<ADC_CONV_START_BIT);
        //read digital data
        *adcDVal=((ADDR>>DIGITAL_DATA_BITS)&1023);
        //equivalent analog representation between 0 to 3.3v
        *eAR=(*adcDVal*(3.3/1024));
}
