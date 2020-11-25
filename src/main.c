//TODO
/*
-Pass actual values to LCD info displays
*/

//LIBRARIES & HEADERS
#include "NU32.h"   //for NU32 board specific functions
#include "useLCD.h"

//TIMERS
//Timer3 is for PWM control of servo
//Timer45 is for LCD display control
//GLOBAL VARIABLES
#define DAYTIME_STATE 1							//Day is 1
#define NIGHTTIME_STATE 0						//Night is 0


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
#define LCD_RS_PIN 28							//NU32 pin B13

//INTERRUPTS

//FUNCTIONS
int init(void)
{	
	NU32_Startup();
	//initServo();
	//initPhotoresistor();
	initLCD();

	return 1;
}

int main(void)
{
	init();

	while (1)
	{
		;
	}
	return 0;
}
