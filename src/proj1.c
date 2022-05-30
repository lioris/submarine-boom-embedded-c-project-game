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
	// איתחולי פורטים
	init();												
	//הצגת פתיח והמתנה להתחלת משחק חדש
	start();									
	// איתחול מטריצות משחק ומסך גרפי
	initGame();										
	 //////////////////////////	
	// לולאה ראשית של המשחק	//
////////////////////////////////////////////////////////
	while(counterMe && counterYou)
	{
		if(turn == My_Turn)
			attack();		// תורנו לתקוף
		else if(turn == En_Turn)
			getBoombCord();	// מחכים לפעולה של שחקן ב
	}
////////////////////////////////////////////////////////

	// הדפסת סוף משחק
	finish();

	while(1);

} 


//------------ איתחולים--------- //
void init() 
{
	initPortB();
	initPortD();
	PORTEout();
	initPortF();
	initPortG();
	setLCD(); 		// מסך רגיל (שתי שורות)
	initLcd();		//מסך גרפי
	cleanlcdg();
	initUART2();
}
//------------------------------//

//---------ממתין להקשת מקש להתחלת משחק----------------//
void start()
{
	char Rdata=0x00;
	turn = 0;
	// הודעות
	PrintStart(); // פתיח - ציור על המסך הגרפי של התחלת משחק
	WriteData("To begin");
	setCursor(0xC0);
	WriteData("press '*'");

	// המתנה למקש
	do{
		key=scan_key();
		if(key == '*')
		{
			U2TXREG = START;
			// ממתינים שיסיים לשלוח
			while(!(U2STAbits.TRMT));//Check TRMT

			turn = My_Turn;
		}
		else if(U2STAbits.URXDA)
			Rdata = U2RXREG;
	}	while( (!(turn)) && (!(Rdata)) );

	// בודקים אם הוקש מקש או התקבל מידע
	//	if( turn ) 
	if(Rdata)
		turn = En_Turn;
}
//-------------------------------------//


//------------שליחת נ"צ לשחקן השני-----------//
void attack()
{
	char HitMiss;

	// קליטת נ"צ מהלוח המקשים 
	do{
		cordXY[0]=getKey();
		cordXY[1]=getKey();
	  }while(!(checkCord()));

	// שליחת הנ"צ בתקשורת לשחקן השני 	
	sendCord(cordXY[0],cordXY[1]);			
	//מחכים לתגובה - פגע/לא פגע
	HitMiss = waitToAns();
	// עדכון מטריצת המשחק
	UpdataMat(HitMiss,2);

	if(HitMiss == MISS)
		turn = En_Turn;	// לא פגע - התור עובר
	else
		counterYou--; 	//פגע - תור נוסף ומורידים אחד לאויב. 
}
//-------------------------------------------//

//---------------קבלת נ"צ ---------------//
void getBoombCord()
{
	clearLcd();
// ממתין לנ"צ משחקן מולנו
	WriteData("wait to cord!");
	//waitToCord(cordXY[0],cordXY[1]);
			cordXY[0] = waitToCord();
			cordXY[1] = waitToCord();

// הדפסה למסך
	printCord(cordXY[0], cordXY[1]);
//בדיקת פגיעה
	// נפעגנו
	if( CheckBox() )
	{	// עדכון מטריצת משחק
		UpdataMat(HIT,1);
		counterMe--;		// מוריד לנו במונה והאויב מקבל תור נוסף
		//החזרת תשובה על פגיעה
		sendAns(HIT);
	}
	// לא נפגענו
	else	
	{	// החזרת תשובה שלא נפגענו
		turn = My_Turn;		// תורנו
		sendAns(MISS);
	} 

}
//------------------------------------//



//----------מעדכן לוח משחק גרפי----------//
void UpdataMat(char HitMiss,int cs)
{
	int x = cordXY[0];
	int y = cordXY[1];
	int part = mySubs[x][y];
	// עדכון לוח ומטריצה שלנו			
	if(cs == 1)
	{
		printHitMe(x,y, mySubs[x][y]);
		mySubs[x][y] = -1;
	}
	// עדכון לוח שלשחקן שמולנו
	else
	{
		if(HitMiss == HIT)
			printHit(x,y);
		else
			printMiss(x,y);
	}
}
//------------------------------//

//------בודק האם היתה פגיעה------//
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

//----------התחלת משחק - איתחול מטריצות ומסך----------------//	
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
	// מאתחל לפי כמות המשבצות כמות הצוללות כפול גודלן
	do{
		for(i=0; i<count; i++){	
			while(!(getSubCord(size))); // חוזרים על הפעולה עד למיקום מוצלח 
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

	// בדיקת מיקום הנ"צ 
	if( checkCord() )
	{
		printPlaceHV(cordXY[0],cordXY[1]);

		do{
			key = scan_key();
		}while( (key!=HORZ) && (key!=VERT) );
		// בודק האם נכנס במסך
		if( checkHV(size, key) ) // key- horz/vert
		{	// ממקם את הצוללת על המסך ומעדכן מטריצה
			place(size, key);
			return 1;	// מוקם בהצלחה
		}
	}

	return 0;	// שגיאה בדרך
}
//---------------------------------------//	

//-----------בדיקת קורדינטות שהוכנסו-----------//	
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

//--------בודק האם צוללת נכנסת למסך---------------------//	
int checkHV(int size,char HZ)
{
	int slots ; // מס המקומות הפנויים לפני גלישה מהמסך
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

	// אם הבדיקות עברו בהצלחה
	return 1;
}
//---------------------------------------//	

//--------בודק את סביבת הצוללת--------//	
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

	// אם כל הבדיקות עברו בהצלחה 
	return 1;
}
//---------------------------------------//	

//-------ממקם את הצוללת על המסך ומעדכן מטריצה------------//	
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
	// הדפסה על מסך גרפי
	// ההדפסה יכולה להתבצע רק אחרי אתחול המטריצה
	printSub(cordXY[0],cordXY[1],size,HV);
}
//---------------------------------------//	

//---------------- אתחול מטריצה אופקית---------------//	
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

//----------------אתחול מטריצה אנכית---------------//	
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