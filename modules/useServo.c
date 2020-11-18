//todo
	//correct setServo math for proper duty cycle

/********************************
SUPLEMENTARY DATA

SERVO SG-90
	-WIRES:
		-BROWN:		GROUND
		-RED:		+5V POWER
		-ORANGE:	PWM
	-SPEED: 0.1s/60deg
	-PWM PERIOD: 20ms/50Hz
	-DUTY CYCLE RANGE
		-MIN = 1ms = 0deg
		-MAX = 2ms = 180deg

PRESCALER OPTIONS TIMERS 2 THRU 5
	0b111 Prescaler of 1:256
	0b110 Prescaler of 1:64
	0b101 Prescaler of 1:32
	0b100 Prescaler of 1:16
	0b011 Prescaler of 1:8
	0b010 Prescaler of 1:4
	0b001 Prescaler of 1:2
	0b000 Prescaler of 1:1

*********************************/

/********************************/
//INCLUDES
/********************************/
#include "NU32.h"          // constants, functions for startup and UART

/********************************/
//DEFINES
/********************************/
#define PWM_TIMER3_PRESCALER = 0b110			//Timer3 prescaler N=4 (1:4)
#define PWM_TIMER3_PERIOD_REGISTER = 24999		//Timer3 period register 
#define PBCLK = 80000000						//System clock frequency
//clock period (T) = (Period Register (PR) + 1) * prescaler (N) / system clock (PBCLK)


/********************************/
//FUNCTIONS
/********************************/

int initServo(int pin)							//Initializes the servo IO & PWM
{
	if (pin == 46)
	{
		T3CONbits.TCKPS = PWM_TIMER3_PRESCALER;		// Timer3 prescaler N=4 (1:4)
		PR3 = PWM_TIMER3_PERIOD_REGISTER;             // 20 ms
		TMR3 = 0;										// initial TMR2 count is 0
		OC1CONbits.OCM = 0b110;						// PWM mode without fault pin; other OC1CON bits are defaults
		OC1CONbits.OCTSEL = 1;						// optional (default)
		OC1RS = 625;									// duty cycle = OC1RS/(PR2+1) = 25%
		OC1R = 625;									// initialize before turning OC1 on; afterward it is read-only
		T3CONbits.ON = 1;								// turn on Timer2
		OC1CONbits.ON = 1;							// turn on OC1
		
		return pin;									//returns pin after init
	}

	return 0;
}

//sets the PWM duty cycle of the servo
void setServo(char dutyCycle)
{
	DC = dutyCycle;							//modify this
	OC1RS = DC * 25000;						//modify this
}