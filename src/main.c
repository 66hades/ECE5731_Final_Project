//LIBRARIES & HEADERS
#include "NU32.h"   //for NU32 board specific functions
#include "useLCD.h"

//TIMERS
//Timer1: Demonstration in place of RTCC
//Timer2: ADC
//Timer3: PWM control of servo
//Timer45: LCD control

//GLOBAL VARIABLES

//PIN DEFINITIONS
/*
#define PHOTORESISTOR_INPUT_PIN = 29			//NU32 pin B14
#define POT_INPUT_PIN = 27						//NU32 pin B12
#define BATT_INPUT_PIN = 30						//NU32 pin B15
#define SOSCO_PIN = 47							//NU32 pin C13
#define SOSCI_PIN = 48							//NU32 pin C14
#define SERVO_PWM_OUTPUT_PIN = 46				//NU32 pin D0
#define RTC_PIN = 42							//NU32 pin D8
#define BUTTon_manual = 43						//NU32 pin D9
#define UART1_RX = 50							//NU32 pin D2
#define UART1_TX = 51							//NU32 pin D3
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
*/

//INTERRUPTS

//FUNCTIONS
int init(void)
{	
	NU32_Startup();
	initServo();
	initLCD();
	initADC();
	initBluetooth();
	initDigital();
	initRTCTimer();

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