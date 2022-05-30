#include <p32xxxx.h>
#include "funcs.h"
#include "signal.h"
#include <math.h>

void signalOut(	unsigned int val)
{
	unsigned char metg8;
	unsigned int TE,PF;
	
	TE = TRISE;
	PF = PORTF;


	TRISE &= 0xFFFFFF00;
	PORTF = 0x05;
	PORTE = val/4;
	Trigger();	
	 
	TRISE = TE;  
	PORTF = PF;
}		

