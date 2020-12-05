//Turn sample timing into interrupt with timer


/********************************/
//INCLUDES
/********************************/
#include "NU32.h"          // constants, functions for startup and UART
#include <stdio.h>
#include "useServo.h"
#include "useDigital.h"

/********************************/
//DEFINES
/********************************/
#define VOLTS_PER_COUNT (3.3/1024) //~3.2mV
#define SAMPLE_TIME 8        // 10 core timer ticks = 200 ns


unsigned int sample12 = 0;		//input for photoresistor
volatile int lightPerc = 0;
unsigned int sample15 = 0;		//input for battery
volatile int battPerc = 0;
unsigned int sample14 = 0;		//input for potentiometer
volatile int anglePos = 0;


/********************************/
//FUNCTIONS
/********************************/

//Timer2 interrupt for sampling ADC
void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2ISR(void) {  // INT step 1: the ISR
	
	setPhoto();
	setPot();
	setBatt();
	
	setServo(getPot());

	IFS0bits.T2IF = 0;              // clear interrupt flag
}

unsigned int adc_sample_convert(int pin) { // sample & convert the value on the given 
                                           // adc pin the pin should be configured as an 
                                           // analog input in AD1PCFG
    unsigned int elapsed = 0, finish_time = 0;
    AD1CHSbits.CH0SA = pin;                // connect chosen pin to MUXA for sampling
    AD1CON1bits.SAMP = 1;                  // start sampling
    elapsed = _CP0_GET_COUNT();
    finish_time = elapsed + SAMPLE_TIME;

    while (_CP0_GET_COUNT() < finish_time) { 
      ;                                   // sample for more than 250 ns
    }

    AD1CON1bits.SAMP = 0;                 // stop sampling and start converting
    while (!AD1CON1bits.DONE) {
      ;                                   // wait for the conversion process to finish
    }

    return ADC1BUF0;                      // read the buffer with the result
}

void initADC(void)
{
	//ADC setup
	AD1PCFGbits.PCFG12 = 0;                 // AN12 is an adc pin (pot)
	AD1PCFGbits.PCFG14 = 0;                 // AN14 is an adc pin (photo)
	AD1PCFGbits.PCFG15 = 0;                 // AN15 is an adc pin (battery)

	AD1CON3bits.ADCS = 2;                   // ADC clock period is Tad = 2*(ADCS+1)*Tpb = 2*3*12.5ns = 75ns
	AD1CON1bits.ADON = 1;                   // turn on A/D converter
	//end ADC setup

	//interrupt setup
	__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
									// INT step 3: setup peripheral
	PR2 = 62499;                    // set period register
	TMR2 = 0;                       // initialize count to 0
	T2CONbits.TCKPS = 0b110;        // set prescaler to 1:64
	T2CONbits.ON = 1;               // turn on Timer23

	IPC2bits.T2IP = 2;              // INT step 4: priority
	IPC2bits.T2IS = 0;              // subpriority
	IFS0bits.T2IF = 0;              // INT step 5: clear interrupt flag
	IEC0bits.T2IE = 1;              // INT step 6: enable interrupt
	__builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
	//end interrupt setup
}

void setPhoto(void)
{
	sample14 = adc_sample_convert(14);    // sample and convert pin 14
	lightPerc = (100.0 / 1023)*sample14;  //sets light percentage from 0 to 100%
	
}

void setBatt(void)
{
	sample15 = adc_sample_convert(15);    // sample and convert pin 15
	battPerc = (100.0 / 1023)*sample15;	  //sets light percentage from 0 to 100%

}

void setPot(void)
{	
	//if in manual mode
	if(manual_auto){
		
	sample12 = adc_sample_convert(12);    // sample and convert pin 12	
	anglePos = (90.0 / 1023)*sample12;		//sets angle between 0 and 90 deg
	}
	else {
		
		if(getDayNight()) {
			anglePos = 0;  //blinds open
		}
		else{
			anglePos = 90; //blinds closed
		}
	}
}

int getPhoto(void)
{
	return lightPerc;
}

int getPot(void)
{
	return anglePos;
}

int getBatt(void)
{
	return battPerc;
}