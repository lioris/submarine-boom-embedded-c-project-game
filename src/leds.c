#include <p32xxxx.h>
#include "funcs.h"

void ledOut(unsigned int data)
{
	unsigned int tempTE,tempPF,tempPE; // int ot int ??

	tempTE = TRISE;
	tempPE = PORTE;
	tempPF = PORTF;

	TRISE &=0xff00; 	//TRISE (0-7 bits) as out	

	///////////////////////////			
	PORTF = 4;
	PORTE = data;
	Trigger();
	//////////////////////////
	TRISE = tempTE;
	PORTE = tempPE;
	PORTF = tempPF;
}

void modeTwo(char direction)
{
	static int  part1=8, part2=16;
	// check switch S3 on/off				
	if( direction == '1' )			//off - menifa out
	{
		if(part2==128){	
			part2=16;
			part1=8;
		}	
		else{
			part2<<=1;
			part1>>=1;
		}
	}
	else						// on - menifa in
	{
		if(part2==16){
			part2=128;
			part1=1;
		}	
		else{
			part2>>=1;
			part1<<=1;
		}
	}

	//PORTE = part1|part2;
	ledOut(part1|part2);
}

void modeOne( char direction)
{
	static int shift=1;

	if( direction == '1' )				// off - shift left
	{
		if(shift == 128){		
			shift=1;
		}
		else{
			shift<<=1;
		}
	}
	else							// on - shift right
	{
		if(shift == 1){
			shift=128;
		}
		else{
			shift>>=1;
		}
	}

	//	PORTE = shift;
	ledOut(shift);
}

void modeZero(  char direction)
{
	static int mone=0;

	if( direction == '1' )			// off - count up	
	{
		if(mone == 255){
			mone=0;
		}
		else{
			mone++;
		}
	}
	else						// on - count down
	{
		if(mone==0){
			mone=255;
		}
		else{
			mone--;
		}
	}
	//	PORTE = mone;
	ledOut(mone);
}