#include "NU32.h"
#include "useLCD.h"

extern volatile int manual_auto = 0; //manual = 1, auto = 0

//manual_auto control
void __ISR(11, IPL2SOFT) BUTTon_manual_ISR(void) {    //Step 1
	
	_CP0_SET_COUNT(0);   //from example on moodle
	
	while( _CP0_GET_COUNT() < 100000 )  ;   // 1 Tick of Core Timer = 25ns -> 2.5ms = 100000 * 25ns
	
	if(!PORTDbits.RD11){
		manual_auto = !manual_auto;
	}

    IFS0bits.INT2IF = 0;            // clear interrupt flag IFS0<3>	
}

//day_night control
void __ISR(19, IPL2SOFT) BUTTon_day_ISR(void) {    //Step 1
	
	_CP0_SET_COUNT(0);   //from example on moodle
	
	while( _CP0_GET_COUNT() < 100000 )  ;   // 1 Tick of Core Timer = 25ns -> 2.5ms = 100000 * 25ns
	
	if(!PORTDbits.RD11){
		day_night = !day_night;
	}

    IFS0bits.INT4IF = 0;            // clear interrupt flag IFS0<3>	
}

void initDigital (void) {
	
	__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
	
  //sw_manual_auto
  INTCONbits.INT2EP = 0;			//INT 0 triggers on falling edge  //step 3
  IPC2bits.INT2IP = 2;				//step 4
  IPC2bits.INT2IS = 1;				//step 4 cont.
  IFS0bits.INT2IF = 0;  			//step 5
  IEC0bits.INT2IE = 1;  			//step 6
  
  //sw_day_night
  INTCONbits.INT4EP = 0;			//INT 0 triggers on falling edge  //step 3
  IPC4bits.INT4IP = 2;				//step 4
  IPC4bits.INT4IS = 1;				//step 4 cont.
  IFS0bits.INT4IF = 0;  			//step 5
  IEC0bits.INT4IE = 1;  			//step 6
  
  __builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
	
}