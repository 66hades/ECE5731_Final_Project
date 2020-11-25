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

#define MSG_LEN_UPPER 16			//Upper row is 16 char long
#define MSG_LEN_LOWER 16			//Lower row is 16 char long

//functions

void main(void)
{
	LCD_Setup();
	initLCD();

	int i = 160000000;
	char infoFlag = 0;
	
	while (1)
	{
		switch (infoFlag) {
		case 0:
			timeStatusLCD(1);
			break;
		case 1:
			lightLevelLCD(45);
			break;
		case 2:
			servoPosLCD(86);
			break;
		case 3:
			batteryLvlLCD(53);
			break;
		default:
			infoFlag = 0;
		}

		while (i > 1)
		{
			i--;
		}

		infoFlag++;

		if (infoFlag > 3)
		{
			infoFlag = 0;
		}

		i = 160000000;
	}
}

void initLCD(void)
{
	LCD_Setup();
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