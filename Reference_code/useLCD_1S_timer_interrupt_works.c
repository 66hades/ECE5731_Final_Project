//includes
//#include "NU32.h"			// constants, funcs for startup and UART
#include "LCD.h"			//uses given LCD library for setup
#include "NU32.h"

#define MSG_LEN_UPPER 16			//Upper row is 16 char long
#define MSG_LEN_LOWER 16			//Lower row is 16 char long

volatile char infoFlag = 0;
//functions
void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) {  // INT step 1: the ISR
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

	infoFlag++;

	if (infoFlag > 3)
	{
		infoFlag = 0;

	}	IFS0bits.T3IF = 0;              // clear interrupt flag
}

void main(void)
{
	NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
	initLCD();



	__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
									// INT step 3: setup peripheral
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

	
	while (1)
	{
		;
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