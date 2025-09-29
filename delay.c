void delay_us(unsigned int dlyUS)
{
    dlyUS*=12;
	while(dlyUS--);
}
void delay_ms(unsigned int dlyMS)
{
    dlyMS*=12000;
	while(dlyMS--);
}
void delay_s(unsigned int dlyS)
{
    dlyS*=1200000;
	  while(dlyS--);
}
