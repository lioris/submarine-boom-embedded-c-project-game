#include <p32xxxx.h>
#include "keyBoard.h"
#include "funcs.h"
#include "lcd.h"

int scan_key(void)
{
	int RUN_ZERO[4] = {0xee,0xdd,0xbb,0x77};

	int scan_key_code_ascii[32]={	0xee,'1',0xde,'2',0xbe,'3',0x7e,'A',
									0xed,'4',0xdd,'5',0xbd,'6',0x7d,'B',
									0xeb,'7',0xdb,'8',0xbb,'9',0x7b,'C',
									0xe7,'*',0xd7,'0',0xb7,'#',0x77,'D'};  

	int i,num_code,keyVal;
	int  column=0;
	int  flag=0;  

	PORTG = 0x00;
	PORTF = 0x07;

	for(i=0;i<20;i++)
	{
		PORTE = RUN_ZERO[column];
		Delay(10);	

		keyVal = PORTB & 0x0F; // קולט את הערך אחרי האפס הרץ
		if(keyVal != 0x0f)	   // אם שונה מהאות אפ אז זה אומר שנקלט תו
		{ 
			flag=1;
			break;
		}

		column++; // אחרת עוברים לעמודה הבאה של האפס הרץ
		if(column==4) column = 0;//טור
	}		

	if(flag==1) // אם הדגל עלה ל1 זה אומר שנקלט תו 
	{				  
		num_code=((RUN_ZERO[column]&0xf0)|(keyVal)); // יצרת הקוד לתו שנקלט
		for(i=0;i<32;i+=2)
		{               
			if(num_code == scan_key_code_ascii[i])
			{
				i=scan_key_code_ascii[i+1];  
				break;
			}		 
		}
	}
	else
		i=0xff;

	return(i);
} 


//---------קבלת נ"צ מהמקלדת-------------//
void getCord(int *cordXY)
{	
	clearLcd();
	WriteData("your turn");
	setCursor(0xc0);
	WriteData("Enter cord ");

	setCursor(0xcc);
	WriteChar('X');

	cordXY[0] = getKey();

	setCursor(0xcc);
	WriteChar('Y');

	cordXY[1] = getKey();
}
//------------------------------//



//---------קבלת קורד' מהמקלדת----------//
int getKey()
{
	unsigned int key;
	while(scan_key() != 0xff);

	do{
		key= scan_key();
	}while(key == 0xff);	

	beep(100);
	while(scan_key() != 0xff);
	
	return key - 48 ;
}
//------------------------------//

