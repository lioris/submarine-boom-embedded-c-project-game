#define S0 1
#define S1 2
#define S2 4
#define S3 8
#define S4 16
#define S5 32
#define S6 64
#define S7 128

#define MODE 0
#define DIRECTION 1
#define SPEED 2

#define LCD_PORT 0
#define SWITCH_PORT 3
#define LED_PORT 4


void Trigger();

void Delay(unsigned int time);

unsigned char SwitchsStatus1();
void Bazzer(unsigned char buzzer_flag);
void beep( unsigned int delay);

void initPortB(void);
void initPortD(void);
void PORTEout();
void PORTEin();
void initPortF(void);
void initPortG(void);