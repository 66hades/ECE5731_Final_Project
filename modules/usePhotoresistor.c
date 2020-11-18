/********************************/
//TODO
/********************************/
//Turn sample timing into interrupt with timer


/********************************/
//INCLUDES
/********************************/
#include "NU32.h"   //for NU32 board specific functions


/********************************/
//DEFINES
/********************************/
#define VOLTS_PER_COUNT (3.3/1024) //~3.2mV
#define CORE_TICK_TIME 25    // nanoseconds between core ticks
#define SAMPLE_TIME 10       // 10 core timer ticks = 250 ns
#define DELAY_TICKS 2000000 // delay 0.05 sec, 20 M core ticks, between messages


/********************************/
//FUNCTIONS
/********************************/

int initPhotosensor(int pin);				//setup photoresistor
{
	if(pin == 30)
	{
		AD1PCFGbits.PCFG15 = 0;                 // AN14 is an adc pin
		AD1CON3bits.ADCS = 2;                   // ADC clock period is Tad = 2*(ADCS+1)*Tpb =
												//                           2*3*12.5ns = 75ns
		AD1CON1bits.ADON = 1;                   // turn on A/D converter

		return pin;
	}

	return 0;
}

unsigned int adc_sample_convert(int pin) { // sample & convert the value on the given 
										   // adc pin the pin should be configured as an 
										   // analog input1 in AD1PCFG
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
	return ADC1BUF0;                      // read the buffer with the result, ranges from 0 to 1023
}