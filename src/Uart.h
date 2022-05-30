#define CTS _RF12
#define RTS _RF13
#define TRTS TRISFbits.TRISF13
#define BRATE ((10000000 / (16 * 2400)) - 1)
#define U_ENABLE 0x8000
#define U_TRX 0x1400


#define MISS 'm'
#define HIT  'h'
#define START 'S'

void initUART2(void);
char sendData(char ch);
char getData(void);

void sendCord(unsigned int cordX, unsigned int cordY);
char waitToCord();

char waitToAns();
void sendAns(char date);