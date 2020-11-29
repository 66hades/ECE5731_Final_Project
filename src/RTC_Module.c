#include "NU32.h"
#include <stdio.h>


void __ISR(_RTCC_VECTOR, IPL3SOFT) __RTCCInterrupt(void)
	{
		
	NU32_WriteUART3("\r\ntest interrupt");
	LATFINV = 0x0003;

	IFS1CLR=0x00008000; // Clear RTCC interrupt flag
	}
	

void main(void) {
	
	NU32_Startup();

	TRISF = 0xFFFC;
	LATFbits.LATF0 = 0;
	LATFbits.LATF1 = 1;


	//RTCC Setup
	__builtin_disable_interrupts();  // INT step 2: disable interrupts at CPU

	NU32_WriteUART3("\r\ntest main 0");
	
	OSCCONbits.SOSCEN = 1;  //turn on secondary clock
	while(!OSCCONbits.SOSCRDY) {;}
	//SYSKEY = 0x0;  //may not need (locks if not already locked)
	SYSKEY = 0xaa996655; // write first unlock key to SYSKEY
	SYSKEY = 0x556699aa; // write second unlock key to SYSKEY
	RTCCONbits.RTCWREN = 1; // set RTCWREN in RTCCONSET
	
	IEC1CLR=0x00008000; // disable RTCC interrupts
	RTCCONbits.ON = 0; // turn off the RTCC	
	while(RTCCON&0x40); // wait for clock to be turned off
	NU32_WriteUART3("\r\ntest main 1");
	IFS1CLR=0x00008000; // clear RTCC flag
	IPC8CLR=0x1f000000; // clear the priority
	IPC8SET=0x0d000000; // Set IPL=3, subpriority 1
	
	RTCTIME=0x16153300; // safe to update time to 16 hr, 15 min, 33 sec
	RTCDATE=0x06102705; // update the date to Friday 27 Oct 2006
	
	//Alarm setup
	RTCALRMCLR=0xCFFF; // clear ALRMEN, CHIME, AMASK and ARPT;
	ALRMTIME=0x16153500; // set alarm time to 16 hr, 15 min, 35 sec
	ALRMDATE=0x06102705; // set alarm date to Friday 27 Oct 2006
	RTCALRMbits.CHIME = 1;  //allows alarm to occur indefintely
	RTCALRMbits.AMASK = 0b0001;  //sets alarm to happen every 1s
	RTCALRMbits.ALRMEN = 1; // re-enable the alarm
	
	//IEC1SET=0x00008000; // Enable RTCC interrupts
	RTCCONbits.ON = 1; // turn on the RTCC
	RTCCONbits.RTCCLKON = 1;  //Turn on clock
	while(!(RTCCON&0x40)); // wait for clock to be turned on
	//RTCCONbits.RTCOE = 1;
	NU32_WriteUART3("\r\ntest main 2");

	__builtin_enable_interrupts();  // INT step 7: enable interrupts at CPU
	NU32_WriteUART3("\r\ntest main 3");
	//End of RTCC setup

	while(1) {;}


} //main