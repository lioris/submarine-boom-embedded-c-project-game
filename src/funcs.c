#include <p32xxxx.h>
#include "funcs.h"
#define BIT(x) 1<<x


void Trigger(){
	PORTDbits.RD4=1;
	PORTDbits.RD4=0;
}


void Delay(unsigned int time){
	unsigned int i ;
	for(i=0; i<time; i++);
}

void Bazzer(unsigned char buzzer_flag)
{
	if( buzzer_flag )	
		PORTGbits.RG15= ~(PORTGbits.RG15);
	else
		PORTGbits.RG15=0;
}

void beep( unsigned int delay)
{
	PORTGbits.RG15= 1;
	Delay(delay);
	PORTGbits.RG15= 0;
}


// switchs status
unsigned char SwitchsStatus1()
{
	unsigned int tempTE,tempPF; // int(32) or short(16)
	unsigned char metagim;

	tempTE = TRISE;
	tempPF = PORTF;

	TRISE |= 0xff;			// make port E as input
	PORTF = SWITCH_PORT;
	////////////////////////////	
	Trigger();
	metagim = PORTE;	
	///////////////////////////////
	TRISE = tempTE;
	PORTF = tempPF;

	return metagim;
}


///////////////////////////////////////////////
//				PORTS INIT					 //
///////////////////////////////////////////////
void initPortB(void)
{
	unsigned int portMap;
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	portMap |= 0xF; 
	TRISB = portMap;

	// RB0-RB3 - key-board pins - digital
	// RB15 - LCD pin - digital
	// RB4,RB8-RB13 - ADC pins - analog
	AD1PCFG = 0xC0EF; //Select PORTB to be digital port input

	// for key-board
	CNCONbits.ON = 0;
	CNEN = 0x3C;
	CNPUE = 0x3C;//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1;
}

void initPortD(void)
{
	TRISDCLR = BIT(4);
	TRISDCLR = BIT(5);
	TRISDCLR = BIT(7);
}

void PORTEout() {TRISE &= 0xFFFFFF00; }
void PORTEin()  {TRISE |= 0x000000FF; }

void initPortF(void)
{	// RF0-RF2,RF8 - decoder - out
	TRISF;
	TRISFCLR = BIT(0);
	TRISFCLR = BIT(1);
	TRISFCLR = BIT(2);
	TRISFCLR = BIT(8);
}

void initPortG(void)
{	
	//RG15 - bazzer - out, RG0-RG1 - decoder - out
	TRISGCLR = BIT(0);
	TRISGCLR = BIT(1);
	TRISGCLR = BIT(15);
	PORTGbits.RG15 = 0;
}
///////////////////////////////////////////////////////