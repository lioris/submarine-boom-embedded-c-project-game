#include <p32xxxx.h>
#include "Uart.h"                       

void initUART2(void)
{
	U2BRG = BRATE;
	U2MODE = U_ENABLE;	//Enable Uart for 8-bit Data, //no Parity, and 1 Stop bit
	U2STASET = U_TRX;		//Enable Transmitter and Receiver
	TRTS = 0;			//Make RTS an output pin
	RTS = 0;			//ready //TO Set "not ready" -  RTS=1 
}

char sendData(char ch)
{
//	while(CTS);
	U2TXREG = ch;
	while(!(U2STAbits.TRMT));	//Check TRMT
	return ch;
}


char getData(void)
{
//	RTS = 0; // requset to send // "im ready to recive"
	while(!(U2STAbits.URXDA)); // מחכה למילה
//	RTS = 1;
	return U2RXREG;
}




//------שליחת נ"צ לשחקן השני------//
void sendCord(unsigned int cordX, unsigned int cordY)
{// convert int x,t to chercter
	char x = cordX + 48;
	char y = cordY + 48;

	sendData(x);
	Delay(10); // השהייה לכל מקרה - שלא תהיה התנגשות
	// עד שאיקס לא נשלח לא שולחים את הנקודה שניה
	sendData(y);
}
//------------------------------//

//----------ממתין לנ"צ מהשחקן השני--------------//
char waitToCord()
{
	char cord;

	cord = getData();
	
	return cord -48;

}
//------------------------------//

//---------ממתין לתגובה מהשחקן השני-------------//
char waitToAns()
{
	return getData();
}
//------------------------------//

//------------------------------//
void sendAns(char date)
{
	sendData(date);
}
//------------------------------//
