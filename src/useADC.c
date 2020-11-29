/********************************/
//TODO
/********************************/
//Turn sample timing into interrupt with timer


/********************************/
//INCLUDES
/********************************/
#include "NU32.h"          // constants, functions for startup and UART
#include <stdio.h>

/********************************/
//DEFINES
/********************************/
#define VOLTS_PER_COUNT (3.3/1024) //~3.2mV
//#define CORE_TICK_TIME 25    // nanoseconds between core ticks
//#define SAMPLE_TIME 10       // 10 core timer ticks = 250 ns
//#define DELAY_TICKS 2000000 // delay 0.05 sec, 20 M core ticks, between messages
#define SAMPLE_TIME 8        // 10 core timer ticks = 200 ns
#define DELAY_TICKS 1000000 // delay 1 sec, 40 M core ticks, between messages


unsigned int sample12 = 0;		//input for photoresistor
int lightPerc = 0;
unsigned int sample14 = 0;		//input for potentiometer
int anglePos = 0;


/********************************/
//FUNCTIONS
/********************************/

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
	AD1PCFGbits.PCFG12 = 0;                 // AN12 is an adc pin (pot)
	AD1PCFGbits.PCFG14 = 0;                 // AN14 is an adc pin (photo)
	AD1CON3bits.ADCS = 2;                   // ADC clock period is Tad = 2*(ADCS+1)*Tpb = 2*3*12.5ns = 75ns
	AD1CON1bits.ADON = 1;                   // turn on A/D converter
}

void getPhoto(void)
{
	sample12 = adc_sample_convert(12);    // sample and convert pin 12
	lightPerc = (100 / 1023)*sample12;		//sets light percentage from 0 to 100%

	//delays reading once ever 1/80 seconds
	_CP0_SET_COUNT(0);
	elapsed = _CP0_GET_COUNT();

	while (_CP0_GET_COUNT() < DELAY_TICKS) 
	{
		;
	}
	//end delay

	return 0;

}

void getPot(void)
{
	sample14 = adc_sample_convert(14);    // sample and convert pin 14
	anglePos = (90 / 1023)*sample14;		//sets angle between 0 and 90 deg
}
