
void WRITELCD(unsigned char byte);

void cmdLCD(unsigned char byte);

void charLCD(unsigned char byte);

void INIT_LCD(void);

void strLCD(char *s);

void u32LCD(int num);

void s32LCD(int num);

void hexLCD(unsigned int num);

void octLCD(unsigned int num);

void binLCD(unsigned int num,int bits);

