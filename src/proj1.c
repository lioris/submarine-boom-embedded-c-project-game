#include <p32xxxx.h>

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#include "funcs.h"
#include "leds.h"
#include "lcd.h"
#include "keyBoard.h"
#include "Glcd.h"
#include "Uart.h"

#define My_Turn 1
#define En_Turn 2


void init(); 
void initGame();
void start();
void finish();

void UpdataMat(char HitMiss,int cs);
void initMatHorz(int size);
void initMatVert(int size);


void attack();
void getBoombCord();
int  CheckBox();
int	 getSubCord(int size);

int  checkCord();
int  checkHV(int size,char HZ);
int  checkAround(int size,char HZ);
void place(int size,char HV);

int turn=0, counterMe=20, counterYou=20, buffer;
unsigned int cordXY[2];
int mySubs[7][7] = {0};
char key;


void main(void)
{
	// ������� ������
	init();												
	//���� ���� ������ ������ ���� ���
	start();									
	// ������ ������� ���� ���� ����
	initGame();										
	 //////////////////////////	
	// ����� ����� �� �����	//
////////////////////////////////////////////////////////
	while(counterMe && counterYou)
	{
		if(turn == My_Turn)
			attack();		// ����� �����
		else if(turn == En_Turn)
			getBoombCord();	// ����� ������ �� ���� �
	}
////////////////////////////////////////////////////////

	// ����� ��� ����
	finish();

	while(1);

} 


//------------ ��������--------- //
void init() 
{
	initPortB();
	initPortD();
	PORTEout();
	initPortF();
	initPortG();
	setLCD(); 		// ��� ���� (��� �����)
	initLcd();		//��� ����
	cleanlcdg();
	initUART2();
}
//------------------------------//

//---------����� ����� ��� ������ ����----------------//
void start()
{
	char Rdata=0x00;
	turn = 0;
	// ������
	PrintStart(); // ���� - ���� �� ���� ����� �� ����� ����
	WriteData("To begin");
	setCursor(0xC0);
	WriteData("press '*'");

	// ����� ����
	do{
		key=scan_key();
		if(key == '*')
		{
			U2TXREG = START;
			// ������� ������ �����
			while(!(U2STAbits.TRMT));//Check TRMT

			turn = My_Turn;
		}
		else if(U2STAbits.URXDA)
			Rdata = U2RXREG;
	}	while( (!(turn)) && (!(Rdata)) );

	// ������ �� ���� ��� �� ����� ����
	//	if( turn ) 
	if(Rdata)
		turn = En_Turn;
}
//-------------------------------------//


//------------����� �"� ����� ����-----------//
void attack()
{
	char HitMiss;

	// ����� �"� ����� ������ 
	do{
		cordXY[0]=getKey();
		cordXY[1]=getKey();
	  }while(!(checkCord()));

	// ����� ��"� ������� ����� ���� 	
	sendCord(cordXY[0],cordXY[1]);			
	//����� ������ - ���/�� ���
	HitMiss = waitToAns();
	// ����� ������ �����
	UpdataMat(HitMiss,2);

	if(HitMiss == MISS)
		turn = En_Turn;	// �� ��� - ���� ����
	else
		counterYou--; 	//��� - ��� ���� �������� ��� �����. 
}
//-------------------------------------------//

//---------------���� �"� ---------------//
void getBoombCord()
{
	clearLcd();
// ����� ��"� ����� �����
	WriteData("wait to cord!");
	//waitToCord(cordXY[0],cordXY[1]);
			cordXY[0] = waitToCord();
			cordXY[1] = waitToCord();

// ����� ����
	printCord(cordXY[0], cordXY[1]);
//����� �����
	// ������
	if( CheckBox() )
	{	// ����� ������ ����
		UpdataMat(HIT,1);
		counterMe--;		// ����� ��� ����� ������ ���� ��� ����
		//����� ����� �� �����
		sendAns(HIT);
	}
	// �� ������
	else	
	{	// ����� ����� ��� ������
		turn = My_Turn;		// �����
		sendAns(MISS);
	} 

}
//------------------------------------//



//----------����� ��� ���� ����----------//
void UpdataMat(char HitMiss,int cs)
{
	int x = cordXY[0];
	int y = cordXY[1];
	int part = mySubs[x][y];
	// ����� ��� ������� ����			
	if(cs == 1)
	{
		printHitMe(x,y, mySubs[x][y]);
		mySubs[x][y] = -1;
	}
	// ����� ��� ������ ������
	else
	{
		if(HitMiss == HIT)
			printHit(x,y);
		else
			printMiss(x,y);
	}
}
//------------------------------//

//------���� ��� ���� �����------//
int CheckBox()
{
	int x = cordXY[0];
	int y = cordXY[1];

	if( mySubs[x][y] == -1)
		return 0;

	if( mySubs[x][y] != 0)
		return 1;
	else
		return 0;
}
//------------------------------//




//----------------------------------------//
void finish()
{
	clearLcd();
	cleanlcdg();
	// win
	if(counterMe){
		printWin();
		WriteData("You WIN THE GAME");
	}
	//lose
	else{
		printLOSE();
		WriteData("You LOSE THE GAME");
	}
}
//---------------------------------------//	

//----------����� ���� - ������ ������� ����----------------//	
void initGame()
{
	char key;
	int i, count=1, size=4;
	
	printBoard();

	clearLcd();
	WriteData("# to init Mat");

	do{
		key= scan_key();
	}while(key != '#');	
	// ����� ��� ���� ������� ���� ������� ���� �����
	do{
		for(i=0; i<count; i++){	
			while(!(getSubCord(size))); // ������ �� ������ �� ������ ����� 
			}
		count++;
		size--;
	}while(size>0);
}
//---------------------------------------//	

//---------------------------------------//	
int	getSubCord(int size)
{
	char key;

	printPlaceSub(size);

	cordXY[0] = getKey();
	cordXY[1] = getKey();

	// ����� ����� ��"� 
	if( checkCord() )
	{
		printPlaceHV(cordXY[0],cordXY[1]);

		do{
			key = scan_key();
		}while( (key!=HORZ) && (key!=VERT) );
		// ���� ��� ���� ����
		if( checkHV(size, key) ) // key- horz/vert
		{	// ���� �� ������ �� ���� ������ ������
			place(size, key);
			return 1;	// ���� ������
		}
	}

	return 0;	// ����� ����
}
//---------------------------------------//	

//-----------����� ��������� �������-----------//	
int  checkCord()
{
	int x = cordXY[0];  
	int y = cordXY[1]; 

	if ( (x>0) && (x<8))
		if ( (y>0) && (y<8) )
			return 1;
	return 0;
}
//---------------------------------------//	

//--------���� ��� ����� ����� ����---------------------//	
int checkHV(int size,char HZ)
{
	int slots ; // �� ������� ������� ���� ����� �����
	int cord;

	if(HZ == HORZ)
		cord = cordXY[1];  // X
	else
		cord = cordXY[0];  // Y

	slots = 8 - cord;
	if(size > slots) // not enough empty slots
		return 0;
	if(!(checkAround(size,HZ)))
		return 0;

	// �� ������� ���� ������
	return 1;
}
//---------------------------------------//	

//--------���� �� ����� ������--------//	
int checkAround(int size,char HZ)
{
	int x = cordXY[0];
	int y = cordXY[1];
	int i;

	if( HZ == HORZ)
	{
		if(y>1)
			if(mySubs[x][y-1] != 0)		
				return 0;
		if(y<7){
			if((y+size)<8){
				if(mySubs[x][y+size] != 0)
					return 0;
			}
		}	
		if(x>1)
		{
			for(i=0; i<size;i++)
			{
				if(mySubs[x-1][y+i] != 0)
					return 0;
			}
		}
		if(x<7)
		{
			for(i=0; i<size;i++)
			{
				if(mySubs[x+1][y+i] != 0)
					return 0;
			}
		}	
	}
	// VERT
	else
	{
		if(x>1)
			if(mySubs[x-1][y] != 0)		
				return 0;
		if(x<7){
			if((x+size)<8){
				if(mySubs[x+size][y] != 0)
					return 0;
			}
		}
		if(y>1)
		{
			for(i=0; i<size;i++)
			{
				if(mySubs[x+i][y-1] != 0)
					return 0;
			}
		}
		if(y<7)
		{
			for(i=0; i<size;i++)
			{
				if(mySubs[x+i][y+1] != 0)
					return 0;
			}
		}	
	}		

	// �� �� ������� ���� ������ 
	return 1;
}
//---------------------------------------//	

//-------���� �� ������ �� ���� ������ ������------------//	
void place(int size,char HV)
{
	int x = cordXY[0];
	int y = cordXY[1];

	if( size == 1 )
		mySubs[x][y] = ONE_SUB	;
	else if( HV == HORZ)
		initMatHorz(size);
	else 
		initMatVert(size);
	// ����� �� ��� ����
	// ������ ����� ������ �� ���� ����� �������
	printSub(cordXY[0],cordXY[1],size,HV);
}
//---------------------------------------//	

//---------------- ����� ������ ������---------------//	
void initMatHorz(int size)
{
	int x = cordXY[0];
	int y = cordXY[1];
	size-=2; // one for the first and one for the end

	mySubs[x][y] = LEFT;

	while(size>0)
	{
		y++;
		mySubs[x][y] = HORZ_CENTER;
		size--;
	}

	y++;
	mySubs[x][y] = RIGHT;
}
//---------------------------------------//	

//----------------����� ������ �����---------------//	
void initMatVert(int size)
{
	int x = cordXY[0];
	int y = cordXY[1];
	size-=2; // one for the first and one for the end

	mySubs[x][y] = UP;

	while(size>0)
	{
		x++;
		mySubs[x][y] = VERT_CENTER;
		size--;
	}
	x++;
	mySubs[x][y] = DOWN;
}
//---------------------------------------//	