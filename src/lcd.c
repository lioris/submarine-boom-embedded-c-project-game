#include <p32xxxx.h>
#include "lcd.h"
#include "funcs.h"
#include "keyBoard.h"

void setLCD()
{
	unsigned int TE,PE,PF;
	char control[6]={0x38,0x38,0x38,0x0e,0x06,0x1};
	int i,j;

	TE = TRISE;
	PE = PORTE;
	PF = PORTF;

	TRISB &= 0xFFFF7FFF; // RB15 as out	
	TRISD &= 0xFFFFFFCF; // PORTD - 4,5 as out
	TRISE &= 0xFFFFFF00; // PORTE - (0-7) as out
	AD1PCFG |=0x8000; // RB15-Digital

	PORTDbits.RD5 =0;// LCD_WRITE;	
	PORTBbits.RB15 =0;// LCD_CONTROL;	//rs - control
	PORTF = LCD_PORT;

	// setting
	for(i=0; i<6; i++)
	{
		PORTE=control[i];
		Trigger();  
		busy();
	}

	TRISE =	TE;
	PORTE =	PE;
	PORTF =	PF; 

}

void LCDcontrol(unsigned int theOp)
{
	char RD,RS;
	unsigned int tempTE, tempPF;

	RD = PORTDbits.RD5;
	RS = PORTBbits.RB15;
	tempTE = TRISE;
	tempPF = PORTF;
	////////////////////////////////////
	TRISE &= 0xff00;
	PORTF = 0;//LCD_PORT;
	PORTDbits.RD5 = LCD_WRITE ;
	PORTBbits.RB15 = LCD_CONTROL ; 
	////////////////////////////////////
	PORTE = theOp;
	Trigger();
	busy();
	///////////////////////////////////
	PORTF = tempPF;
	TRISE = tempTE;  	
	PORTDbits.RD5 = RD; 
	PORTBbits.RB15 = RS;
}

void setCursor(unsigned int address){
	LCDcontrol(address);
}

void clearLcd(){
	LCDcontrol(0x1);
}

void returnHome(){
	LCDcontrol(0x2);
}

void busy(void)
{ 
	char RD,RS;
	unsigned int tempTE;

	RD = PORTDbits.RD5;
	RS = PORTBbits.RB15;
	tempTE = TRISE;
	///////////////////////////////////////
	PORTDbits.RD5 =1;// LCD_READ; 
	PORTBbits.RB15 =0;// LCD_CONTROL; 
	TRISE |= 0x80;		// BIT 7 AS IN
	//	TRISESET = 0x80;
	//////////////////////////////////////	
	do{
		PORTDbits.RD4=1;//enable=1
		PORTDbits.RD4=0;//enable=0
	}while(PORTEbits.RE7);// BF בדיקה דגל
	//////////////////////////////////////
	PORTDbits.RD5  = RD; 
	PORTBbits.RB15 = RS;
	TRISE=tempTE;   
}


void WriteData(char *string)
{
	char RD,RS;
	unsigned int tempTE, tempPF;

	RD = PORTDbits.RD5;
	RS = PORTBbits.RB15;
	tempTE = TRISE;
	tempPF = PORTF;
	////////////////////////////////////
	TRISE &= 0xff00;
	PORTF=0;
	PORTDbits.RD5 = LCD_WRITE ;
	PORTBbits.RB15 = LCD_DATA ; 
	////////////////////////////////////
	while(*string)
	{
		PORTE = *string;
		Trigger();
		busy();
		string++;
	}
	///////////////////////////////////
	PORTF = tempPF;
	TRISE = tempTE;  	
	PORTDbits.RD5 = RD; 
	PORTBbits.RB15 = RS;	
}


void WriteChar(char ch)
{
	char RD,RS;
	unsigned int TE, PF;

	RD = PORTDbits.RD5;
	RS = PORTBbits.RB15;
	TE = TRISE;
	PF = PORTF;
	////////////////////////////////////
	TRISE &= 0xff00;
	PORTF=0;
	PORTDbits.RD5 = LCD_WRITE ;
	PORTBbits.RB15 = LCD_DATA ; 
	////////////////////////////////////
	PORTE = ch;
	Trigger();
	busy();
	///////////////////////////////////
	PORTF = PF;
	TRISE = TE;  	
	PORTDbits.RD5 = RD; 
	PORTBbits.RB15 = RS;
}


void printCord(unsigned int cordX, unsigned int cordY)
{
	char x = cordX+48;
	char y = cordY+48;

	clearLcd();
	WriteData("the boomb cord is");
	setCursor(0xc0);
	WriteData("( ");
	WriteChar(x);
	WriteData(" , ");
	WriteChar(y);
	WriteData(" )");
}

void printPlaceSub(int size)
{
	clearLcd();
	WriteData("place   size sub");
	setCursor(0x87);
	WriteChar(size+48);
	setCursor(0xc0);
	WriteData("enter (x,y)");
}

void printPlaceHV(unsigned int CordX,unsigned int CordY)
{
	char x = CordX + 48;
	char y = CordY + 48;
	clearLcd();
	WriteData("the cord ( , )");
	setCursor(0x8a);
	WriteChar(x);
	setCursor(0x8c);
	WriteChar(y);
	setCursor(0xc0);
	WriteData("1)horz 2)vert");
}