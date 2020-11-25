//LIBRARIES & HEADERS
#include "NU32.h"   //for NU32 board specific functions
#include "useLCD.h"

//TIMERS
//Timer3 is for PWM control of servo
//Timer45 is for LCD display control
//GLOBAL VARIABLES
#define DAYTIME_STATE 1							//Day is 1
#define NIGHTTIME_STATE 0						//Night is 0

volatile char infoFlag = 2;			//0 day or night; 1 photo; 2 servo; 3 battery


//PIN DEFINITIONS
#define PHOTORESISTOR_INPUT_PIN = 29			//NU32 pin B14
#define SERVO_PWM_OUTPUT_PIN = 46				//NU32 pin D0
#define RTC_PIN = 42							//NU32 pin D8
#define LCD_0_PIN 60							//NU32 pin E0
#define LCD_1_PIN 61							//NU32 pin E1
#define LCD_2_PIN 62							//NU32 pin E2
#define LCD_3_PIN 63							//NU32 pin E3
#define LCD_4_PIN 64							//NU32 pin E0
#define LCD_5_PIN 1								//NU32 pin E0
#define LCD_6_PIN 2								//NU32 pin E6
#define LCD_7_PIN 3								//NU32 pin E7
#define LCD_E_PIN 52							//NU32 pin D4
#define LCD_R_W_PIN 53							//NU32 pin D5
#define LCD_RS_PIN 28							//NU32 pin 13

//INTERRUPTS
//LCD timer interrupt
void __ISR(20, IPL5SOFT) infoTimerISR(void)
{
	timeStatusLCD(1);
	switch (infoFlag) {
	case 0:
		timeStatusLCD(1);
	case 1:
		lightLevelLCD(45);
	case 2:
		servoPosLCD(67);
	case 3:
		batteryLvlLCD(23);
	}

	//resets flag to 0 if greater than 4
	infoFlag++;
	if (infoFlag > 3)
	{
		infoFlag = 0;
	}

	IFS0bits.T5IF = 0;              // clear interrupt flag
}


//FUNCTIONS
int init(void)
{	
	NU32_Startup();
	//initServo();
	//initPhotoresistor();
	initLCD();

	return 1;
}

//INTERRUPTS
//LIST HERE

int main(void)
{
	while (1)
	{
		;
	}
	return 0;
}
