#include "NU32.h"
#include <stdio.h>

volatile int count = 0;
volatile char msg [20];

//timer interrupt
void __ISR(_TIMER_1_VECTOR, IPL1SOFT) Timer_ISR(void) {    //Step 1
		
	//causes interrupt to wait 50 cycles (essentially slows interrupt from 0.2 seconds to 10 seconds)	
	if (count < 50) {
		count++;
	}		
		
	//what we want the interrupt to actually do	
	else {
		LATFINV = 0b11;  //inverts LEDs for testing
		count = 0;
	}
		
	//outputs count to serial for testing (program won't work unless putty is open if this code is left in)
	/*sprintf(msg, "\r\n%d", count);
	NU32_WriteUART3(msg); */
		
	IFS0bits.T1IF = 0;  //clears interrupt flag
}


void main(void) {
	
	NU32_Startup();
 
	//LED setup
	TRISF = 0xFFFC;
	LATFbits.LATF0 = 0;
	LATFbits.LATF1 = 1;

	__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU

	//timer setup                   // INT step 3: setup peripheral
	PR1 = 62499;                    //             set period register
	TMR1 = 0;                       //             initialize count to 0
	T1CONbits.TCKPS = 0b11;         //             set prescaler to 256
	T1CONbits.ON = 1;               //             turn on Timer1
	IPC1bits.T1IP = 1;              // INT step 4: priority
	IPC1bits.T1IS = 0;
	IFS0bits.T1IF = 0;              // INT step 5: clear interrupt flag   
	IEC0bits.T1IE = 1;              // INT step 6: enable interrupt
	
	__builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
  
    while (1) {;}
	
}  //main