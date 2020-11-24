//todo

//includes

//defines

//functions
	//init

/* The following code example will update the RTCC time and date. */
/*assume the secondary oscillator is enabled and ready, i.e. OSCCON<1>=1, OSCCON<22>=1,
and RTCC write is enabled i.e. RTCWREN (RTCCON<3>) =1;*/
unsigned long time=0x04153300;// set time to 04 hr, 15 min, 33 sec
unsigned long date=0x06102705;// set date to Friday 27 Oct 2006
RTCCONCLR=0x8000; // turn off the RTCC
while(RTCCON&0x40); // wait for clock to be turned off
RTCTIME=time; // safe to update the time
RTCDATE=date; // update the date
RTCCONSET=0x8000; // turn on the RTCC
while(!(RTCCON&0x40)); // wait for clock to be turned on
// can disable the RTCC write

/* The following code example will update the RTCC time and date. */
/*assume RTCC write is enabled i.e. RTCWREN (RTCCON<3>) =1; */
unsigned long time=0x04153300;// set time to 04 hr, 15 min, 33 sec
unsigned long date=0x06102705;// set date to Friday 27 Oct 2006
asm volatile (“di”); // disable interrupts, critical section follows
while((RTCCON&0x4)!=0); // wait for not RTCSYNC
RTCTIME=time; // safe to update the time
RTCDATE=date; // update the date
asm volatile (“ei”); // restore interrupts, critical section ended
// can disable the RTCC write

/*
The following code example demonstrates a simple interrupt service routine for RTCC
interrupts. The user’s code at this vector should perform any application specific
operations and must clear the RTCC interrupt flag before exiting.
*/
void__ISR(_RTCC_VECTOR, ipl3) __RTCCInterrupt(void)
{
// ... perform application specific operations
// in response to the interrupt
IFS1CLR=0x00008000; // be sure to clear RTCC interrupt flag
// before exiting the service routine.
}

	/*
The following code example illustrates an RTCC initialization with interrupts enabled.
When the RTCC alarm interrupt is generated, the cpu will jump to the vector assigned to
RTCC interrupt.
*/
/*// assume RTCC write is enabled i.e. RTCWREN (RTCCON<3>) =1;*/
IEC1CLR=0x00008000; // disable RTCC interrupts
RTCCONCLR=0x8000; // turn off the RTCC

while(RTCCON&0x40); // wait for clock to be turned off

IFS1CLR=0x00008000; // clear RTCC existing event
IPC8CLR=0x1f000000; // clear the priority
IPC8SET=0x0d000000; // Set IPL=3, subpriority 1
IEC1SET=0x00008000; // Enable RTCC interrupts
RTCTIME=0x16153300; // safe to update time to 16 hr, 15 min, 33 sec
RTCDATE=0x06102705; // update the date to Friday 27 Oct 2006
RTCALRMCLR=0xCFFF; // clear ALRMEN, CHIME, AMASK and ARPT;

ALRMTIME=0x16154300; // set alarm time to 16 hr, 15 min, 43 sec
ALRMDATE=0x06102705; // set alarm date to Friday 27 Oct 2006

RTCALRMSET=0x8000|0x00000600; // re-enable the alarm, set alarm mask at once per day
RTCCONSET=0x8000; // turn on the RTCC

while(!(RTCCON&0x40)); // wait for clock to be turned on