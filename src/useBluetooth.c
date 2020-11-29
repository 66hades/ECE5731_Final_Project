//todo

//includes
#include "NU32.h"              // constants, functions for startup and UART
#include "useLCD.h"				//for day/night variable
//defines

volatile int flag = 0;

//functions
void __ISR(_UART_1_VECTOR, IPL1SOFT) IntUart1Handler(void) {
	if (IFS0bits.U1RXIF) {       // check if interrupt generated by a RX event
		//code for changing day/night here

		if (U1RXREG == 'N')
		{
			day_night = !day_night;
		}

		//end code for changing day/night here
		IFS0bits.U1RXIF = 0;       // clear the RX interrupt flag
	}

	/*code from Book example: uart_int.c
	else if (IFS1bits.U3TXIF) { // if it is a TX interrupt 
	}
	else if (IFS1bits.U3EIF) {  // if it is an error interrupt. check U3STA for reason
	}
	*/
}

void initBluetooth(void)
{
	NU32_LED1 = 0;
	NU32_LED2 = 0;

	__builtin_disable_interrupts();

	// set baud to 9600, to match terminal emulator; use default 8N1 of UART
	U1MODEbits.BRGH = 0;
	U1BRG = ((NU32_SYS_FREQ / 9600) / 16) - 1;

	// configure TX & RX pins 
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;

	// configure using RTS and CTS
	U1MODEbits.UEN = 2;

	// configure the UART interrupts
	U1STAbits.URXISEL = 0x0; // RX interrupt when receive buffer not empty
	IFS0bits.U1RXIF = 0;     // clear the rx interrupt flag.  for 
							 // tx or error interrupts you would also need to clear
							 // the respective flags
	IPC6bits.U1IP = 1;       // interrupt priority
	IEC0bits.U1RXIE = 1;     // enable the RX interrupt

							 // turn on UART1
	U1MODEbits.ON = 1;
	__builtin_enable_interrupts();
}
