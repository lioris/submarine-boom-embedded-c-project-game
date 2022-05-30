#define LCD_CONTROL 0
#define LCD_DATA 1

#define LCD_WRITE 0
#define LCD_READ 1

void busy(void);
void setLCD();
void WriteData(char *string);
void WriteChar(char ch);
void setCursor(unsigned int address);
void clearLcd();
void returnHome();

void printCord(unsigned int cordX, unsigned int cordY);

void printPlaceSub(int size);
void printPlaceHV(unsigned int CordX,unsigned int CordY);