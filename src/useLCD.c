/*todo
-add interrupts at 5 second intervals to switch what info is displayed
-If time, custom day/night chars for states
*/

/*SUPPLEMENTARY INFO
LCD screen is 16 char wide by 2 rows
LCD screen needs 5M delay to display clearly

// set up the parallel master port (PMP) to control the LCD
// pins RE0 - RE7 (PMD0 - PMD7) connect to LCD pins D0 - D7
// pin RD4 (PMENB) connects to LCD pin E
// pin RD5 (PMRD/PMWR) Connects to LCD pin R/W
// pin RB13 (PMA10) Connects to RS.
// interrupts will be disabled while init setup function executes
*/

//includes
//#include "NU32.h"			// constants, funcs for startup and UART
#include<stdio.h>
#include "LCD.h"			//uses given LCD library for setup
#include "NU32.h"
#include "useLCD.h"

//defines
volatile char infoFlag = 0;			//0 day or night; 1 photo; 2 servo; 3 battery
volatile int state;
volatile int lightLvl;
volatile int servoPos;
volatile int battLvl;

//functions
//timer interrupt
void __ISR(_TIMER_3_VECTOR, IPL5SOFT) infoTimerISR(void) 
{
	switch (infoFlag) {
	case 0:
		timeStatusLCD(state);
		break;
	case 1:
		lightLevelLCD(lightLvl);
		break;
	case 2:
		servoPosLCD(servoPos);
		break;
	case 3:
		batteryLvlLCD(battLvl);
		break;
	}

	IFS0bits.T3IF = 0;              // clear interrupt flag
}


void initLCD(void)
{
	LCD_Setup();

	//timer interrupt setup
	__builtin_disable_interrupts();
	T2CONbits.T32 = 1;               // use Timer23 in 32bit mode
	PR2 = 80000000;               //             set period register
	TMR2 = 0;                       //             initialize count to 0
	T2CONbits.TCKPS = 0;            //             set prescaler to 1
	T2CONbits.ON = 1;               //             turn on Timer23

	IPC3bits.T3IP = 5;              // INT step 4: priority
	IPC3bits.T3IS = 0;              //             subpriority
	IFS0bits.T3IF = 0;              // INT step 5: clear interrupt flag
	IEC0bits.T3IE = 1;              // INT step 6: enable interrupt
	
	__builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
	//end setup interrupt
}

void timeStatusLCD(int state)
{
	LCD_Clear();
	LCD_Move(0, 0);
	LCD_WriteString("CURRENT STATE:");
	LCD_Move(1, 0);
	
	if (state)
	{
		LCD_WriteString("DAYTIME");
	}
	else
	{
		LCD_WriteString("NIGHTTIME");
	}
}

void lightLevelLCD(int lightLvl)
{
	char s[MSG_LEN_LOWER];
	sprintf(s, "%d%%", lightLvl);
	LCD_Clear();
	LCD_Move(0, 0);
	LCD_WriteString("LIGHT LEVEL:");
	LCD_Move(1, 0);
	LCD_WriteString(s);
}

void servoPosLCD(int servoPos)
{
	char s[MSG_LEN_LOWER];
	sprintf(s, "%d DEGREES", servoPos);
	LCD_Clear();
	LCD_Move(0, 0);
	LCD_WriteString("SERVO POSITION:");

	LCD_Move(1, 0);
	LCD_WriteString(s);
}

void batteryLvlLCD(int battLvl)
{
	char s[MSG_LEN_LOWER];
	sprintf(s, "%d%%", battLvl);
	LCD_Clear();
	LCD_Move(0, 0);
	LCD_WriteString("BATTERY LEVEL:");

	LCD_Move(1, 0);
	LCD_WriteString(s);
}