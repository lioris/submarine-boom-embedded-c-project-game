#define HORZ '1'
#define VERT '2'

#define LEFT			0
#define HORZ_CENTER 	1
#define RIGHT			2
#define UP				3
#define VERT_CENTER 	4
#define DOWN			5
#define ONE_SUB			6

#define LEFT_X			7
#define HORZ_CENTER_X	8
#define RIGHT_X			9
#define UP_X			10
#define VERT_CENTER_X	11
#define DOWN_X			12
#define ONE_SUB_X		13

void cleanlcdg();
void initLcd(void);
void writeXY(unsigned int x,unsigned int y,int lcd_cs);
void writeLcd(unsigned int num,int lcd_cs);

void printBoard();

void PrintStart();
void printWin();
void printLOSE();

void printHit(int x, int y);
void printMiss(int x, int y);
void printHitMe(int x, int y, int part);
void printBox(unsigned int *box, int cs);
void printSub(int x, int y, int size,char HV);

