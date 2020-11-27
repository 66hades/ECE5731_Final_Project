#include "NU32.h"

volatile int day_night = 1;

void __ISR(19, IPL2SOFT) BUTTon_ISR(void) {    //Step 1
	
	_CP0_SET_COUNT(0);   //from example on moodle
	
	while( _CP0_GET_COUNT() < 100000 )  ;   // 1 Tick of Core Timer = 25ns -> 2.5ms = 100000 * 25ns
	
	if(!PORTDbits.RD11){
		day_night = !day_night;
	}

    IFS0bits.INT4IF = 0;            // clear interrupt flag IFS0<3>	
}

void main(void) {
	
	char msg[100];
	
	NU32_Startup();
	
  __builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
	
  //sw0
  INTCONbits.INT4EP = 0;			//INT 0 triggers on falling edge  //step 3
  IPC4bits.INT4IP = 2;				//step 4
  IPC4bits.INT4IS = 1;				//step 4 cont.
  IFS0bits.INT4IF = 0;  			//step 5
  IEC0bits.INT4IE = 1;  			//step 6
  
  __builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU

	while(1) {
		
	sprintf(msg, "\r\n%d", day_night);
	NU32_WriteUART3(msg);
		
	}

} //main