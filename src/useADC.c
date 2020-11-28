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
#define DELAY_TICKS 40000000 // delay 1 sec, 40 M core ticks, between messages

/********************************/
//FUNCTIONS
/********************************/

/*int initPhotosensor(int pin)				//setup photoresistor
{
	if(pin == 29)
	{
		AD1PCFGbits.PCFG15 = 0;                 // AN14 is an adc pin
		AD1CON3bits.ADCS = 2;                   // ADC clock period is Tad = 2*(ADCS+1)*Tpb =
												//                           2*3*12.5ns = 75ns
		AD1CON1bits.ADON = 1;                   // turn on A/D converter

		return pin;
	}

	return 0;
}*/

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

int main(void) {
  unsigned int sample12 = 0, sample14 = 0, elapsed = 0;
  char msg[100] = {};

  NU32_Startup();                 // cache on, interrupts on, LED/button init, UART init

  AD1PCFGbits.PCFG12 = 0;                 // AN12 is an adc pin (pot)
  AD1PCFGbits.PCFG14 = 0;                 // AN14 is an adc pin (photo)
  AD1CON3bits.ADCS = 2;                   // ADC clock period is Tad = 2*(ADCS+1)*Tpb = 2*3*12.5ns = 75ns
  AD1CON1bits.ADON = 1;                   // turn on A/D converter

  while (1) {
    _CP0_SET_COUNT(0);                    // set the core timer count to zero
    sample12 = adc_sample_convert(12);    // sample and convert pin 12
    sample14 = adc_sample_convert(14);    // sample and convert pin 14
    elapsed = _CP0_GET_COUNT();           // how long it took to do two samples
                                          // send the results over serial
    sprintf(msg, "AN12: %4d (%5.3f volts) \r\n", sample12, sample12 * VOLTS_PER_COUNT);
    NU32_WriteUART3(msg);
    sprintf(msg, "AN14: %4d (%5.3f volts) \r\n", sample14, sample14 * VOLTS_PER_COUNT);
    NU32_WriteUART3(msg);
    
    _CP0_SET_COUNT(0);                    // delay to prevent a flood of messages

    while(_CP0_GET_COUNT() < DELAY_TICKS) {;}
	
  }

  return 0;
}