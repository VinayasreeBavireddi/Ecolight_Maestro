void rtc_init(void);
void rtc_Set_Pre(void);
void ValidateAndSet(char *msg, unsigned int minVal, unsigned int maxVal, void (*setFunc)(unsigned int));
