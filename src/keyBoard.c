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

		keyVal = PORTB & 0x0F; // ���� �� ���� ���� ���� ���
		if(keyVal != 0x0f)	   // �� ���� ����� �� �� �� ���� ����� ��
		{ 
			flag=1;
			break;
		}

		column++; // ���� ������ ������ ���� �� ���� ���
		if(column==4) column = 0;//���
	}		

	if(flag==1) // �� ���� ��� �1 �� ���� ����� �� 
	{				  
		num_code=((RUN_ZERO[column]&0xf0)|(keyVal)); // ���� ���� ��� �����
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


//---------���� �"� �������-------------//
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



//---------���� ����' �������----------//
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

