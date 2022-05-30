#include <p32xxxx.h>
#include "ADC.h"
#include "funcs.h"
#include "lcd.h"
#include "keyBoard.h"
#include "signal.h"

void signalSamp(char key)
{
	unsigned char metg8;
	
	PORTFbits.RF8 = 1;
	SwitchsStatus(&metg8);

	AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling and starts converting
	AD1CON2 = 0;	
	AD1CON3 = 0x0002; // Manual Sample, Tad = 6 TPB
	AD1CON1SET = 0x8000; // turn ADC ON
	
	while(metg8&S7)
	{
		 if(key == '1')
			potnzmoterV();
	else if(key == '2')
			VoltageVal();
	else if(key == '3')
			signalGenarator();

	 SwitchsStatus(&metg8);
	}
	
	PORTFbits.RF8 = 0;
	AD1CON1CLR = 0x8000; // turn ADC OFF
}

void potnzmoterV()
{
	unsigned long i,ADCValue;
	static unsigned long temp = 0;	
	AD1CHS=0x00080000;	//  RB8/AN8 is the input
	AD1CSSL = 0x100;	// input scan selction
	
	AD1CON1SET = 0x0002; // start sampling ...
	for(i=0;i<100000;i++);//DelayNmSec(100); // for 100 mS
	AD1CON1CLR = 0x0002; // start Converting
	while (!(AD1CON1 & 0x0001));// conversion done?
		ADCValue = ADC1BUF0; // yes then get ADC value
	
	if(ADCValue == temp)
		return;
	
	temp = ADCValue;
	ledOut(ADCValue/4);//דחיסה
	ValSampPrintLcd(ADCValue);
}

void VoltageVal()
{
	unsigned long i,ADCValue;
	static unsigned long temp = 0;	
	
	AD1CHS=0x00090000;	//  RB9 is the input
	AD1CSSL = 0x200;	// input scan selction
	
	AD1CON1SET = 0x0002; // start sampling ...
	for(i=0;i<100000;i++);//DelayNmSec(100); // for 100 mS
	AD1CON1CLR = 0x0002; // start Converting
	while (!(AD1CON1 & 0x0001));// conversion done?
		ADCValue = ADC1BUF0; // yes then get ADC value

	if(ADCValue == temp)
		return;

	temp = ADCValue;
	ledOut(ADCValue/4);//דחיסה
	ValSampPrintLcd(ADCValue);
}

void signalGenarator()
{
	unsigned long i,ADCValue;
		
	AD1CHS=0x000a0000;	//  RB10 is the input
	AD1CSSL = 0x400;	// input scan selction
	
	AD1CON1SET = 0x0002; // start sampling ...
	for(i=0;i<100000;i++);//DelayNmSec(100); // for 100 mS
	AD1CON1CLR = 0x0002; // start Converting
	while (!(AD1CON1 & 0x0001));// conversion done?
		ADCValue = ADC1BUF0; // yes then get ADC value


	signalOut(ADCValue);
}
