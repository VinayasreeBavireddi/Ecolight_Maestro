//pin_connect_block.c
#include<LPC21XX.h>
void CfgPortPinFunc(unsigned int portNo,unsigned int pinNo,unsigned int pinFunc)
{
	if(portNo==0)
	{
		if(((int)pinNo>=0) && (pinNo<=15))
		{
			PINSEL0=((PINSEL0&~(3<<pinNo*2))|(pinFunc<<(pinNo*2)));
	  }
	  else if(pinNo>=16 && pinNo<=31)
	  {
			PINSEL1=((PINSEL1&~(3<<((pinNo-16)*2)))|(pinFunc<<((pinNo-16)*2)));
	  }
	}
}
