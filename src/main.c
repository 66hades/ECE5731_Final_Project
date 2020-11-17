//LIBRARIES & HEADERS
#include "NU32.h"   //for NU32 board specific functions

//GLOBAL VARIABLES
//PIN DEFINITIONS
#define PHOTORESISTOR_INPUT_PIN = 29			//NU32 pin B14
#define SERVO_PWM_OUTPUT_PIN = 46				//NU32 pin D0

//FUNCTIONS
void init(void)
{
	NU32_Startup();         //cache on, interrupts on, LED/button init, UART init
	initServo();			//start & setup servo
	initPhotoresistor();	//start & setup photoresistor
}

//INTERRUPTS
//LIST HERE

int main(void)
{
	init();					//start & setup all modules
	return 0;
}
