//LIBRARIES & HEADERS
#include "NU32.h"   //for NU32 board specific functions

//GLOBAL VARIABLES
//PIN DEFINITIONS
#define PHOTORESISTOR_INPUT_PIN = 29			//NU32 pin B14
#define SERVO_PWM_OUTPUT_PIN = 46				//NU32 pin D0
#define RTC_PIN = 42							//NU32 pin D8

//FUNCTIONS
void init(void)
{	
	NU32_Startup();
	//checks all modules to see if they initialized correctly
	if(initServo(SERVO_PWM_OUTPUT_PIN) && initPhotoresistor(PHOTORESISTOR_INPUT_PIN))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//INTERRUPTS
//LIST HERE

int main(void)
{
	if (!init())			//start & setup all modules
	{						//on failure, onboard LEDs will alternate flashing every 1/2 second
		while (1)
		{
			NU32_LED1 = 1;

			for (int i = 40000000)
			{
				NU32_LED1 = !NU32_LED1;
				NU32_LED2 = !NU32_LED2;
			}
		}
	} 
	
						//on successful init, main infinite loop executes
	while (1)				//put main loop stuff here
	{

	}
	
	
	return 0;
}
