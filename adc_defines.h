//adc_defines.h
//clock defines

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADCLK 3000000
#define CLKDIV ((PCLK/ADCLK)-1)
//defines for ADCR SFR

#define CLKDIV_BITS 8
#define PDN_BIT 21
#define ADC_CONV_START_BIT 24
//define for ADDR SFR

#define DIGITAL_DATA_BITS 6
#define DONE_BIT 31
