/*todo
-If time, custom day/night chars for states
-If time, reduce time spent in interrupt Timer5ISR. Change a flag which gets checked and excecuted in main loop
*/

/*SUPPLEMENTARY INFO
LCD screen is 16 char wide by 2 rows
LCD screen needs ~5M delay to display clearly

// set up the parallel master port (PMP) to control the LCD
// pins RE0 - RE7 (PMD0 - PMD7) connect to LCD pins D0 - D7
// pin RD4 (PMENB) connects to LCD pin E
// pin RD5 (PMRD/PMWR) Connects to LCD pin R/W
// pin RB13 (PMA10) Connects to RS.
// LCD pin V0 to GND with 2.4kOhm resistor for contrast
// interrupts will be disabled while init setup function executes
*/

//includes
//#include "NU32.h"			// constants, funcs for startup and UART
#include "LCD.h"			//uses given LCD library for setup
#include "NU32.h"
#include <stdio.h>
#include "useADC.h"

#define MSG_LEN_UPPER 16			//Upper row is 16 char long
#define MSG_LEN_LOWER 16			//Lower row is 16 char long

volatile char infoFlag = 0;			//0 day or night; 1 photo; 2 servo; 3 battery
extern volatile int day_night = 1;  //day = 1, night = 0
volatile char msg[100] = {};

//functions
//Timer45 interrupt for cycling through LCD info 
void __ISR(_TIMER_5_VECTOR, IPL3SOFT) Timer5ISR(void) {  // INT step 1: the ISR
	switch (infoFlag) {
	case 0:
		timeStatusLCD(day_night);
		break;
	case 1:
		lightLevelLCD(getPhoto());
		break;
	case 2:
		servoPosLCD(getPot());
		break;
	case 3:
		batteryLvlLCD(getBatt());
		break;
	default:
		infoFlag = 0;
	}

	infoFlag++;

	if (infoFlag > 3)
	{
		infoFlag = 0;

	}	
	
	sprintf(msg, "Time: %d\t\tLight: %d\t\tServo: %d\t\tBatt: %d\n\r", day_night, getPhoto(), getPot(), getBatt());
	//NU32_WriteUART3(msg);

	IFS0bits.T5IF = 0;              // clear interrupt flag
}

void initLCD(void)
{
	LCD_Setup();

	__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
									// INT step 3: setup peripheral
	T4CONbits.T32 = 1;               // use Timer23 in 32bit mode
	PR4 = 49999999;               //             set period register
	//use 49999999 for 5 seconds
	TMR4 = 0;                       //             initialize count to 0
	T4CONbits.TCKPS = 0b011;            //             set prescaler to 1:8
	T4CONbits.ON = 1;               //             turn on Timer23

	IPC5bits.T5IP = 3;              // INT step 4: priority
	IPC5bits.T5IS = 0;              //             subpriority
	IFS0bits.T5IF = 0;              // INT step 5: clear interrupt flag
	IEC0bits.T5IE = 1;              // INT step 6: enable interrupt
	__builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU

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

int getDayNight(void)
{
	return day_night;
}

void invDayNight(void)
{
	day_night = !day_night;
}