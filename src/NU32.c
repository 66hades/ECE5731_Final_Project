#include "NU32.h"

#define NU32_DESIRED_BAUD 115200    // Baudrate for RS232

//  Make NU32_LED1 and NU32_LED2 pins outputs (NU32_USER is by default an input)
void NU32_Startup() {
  TRISFCLR = 0x0003;  // Make F0 and F1 outputs (LED1 and LED2)
  NU32_LED1 = 1;      // LED1 is off
  NU32_LED2 = 0;      // LED2 is on

  // turn on UART3 without an interrupt
  U3MODEbits.BRGH = 0; // set baud to NU32_DESIRED_BAUD
  U3BRG = ((NU32_SYS_FREQ / NU32_DESIRED_BAUD) / 16) - 1;

  // 8 bit, no parity bit, and 1 stop bit (8N1 setup)
  U3MODEbits.PDSEL = 0;
  U3MODEbits.STSEL = 0;

  // configure TX & RX pins as output & input pins
  U3STAbits.UTXEN = 1;
  U3STAbits.URXEN = 1;
  // configure hardware flow control using RTS and CTS
  U3MODEbits.UEN = 2;

  // enable the uart
  U3MODEbits.ON = 1;
}

// Read from UART3
// block other functions until you get a '\r' or '\n'
// send the pointer to your char array and the number of elements in the array
void NU32_ReadUART3(char * message, int maxLength) {
  char data = 0;
  int complete = 0, num_bytes = 0;
  // loop until you get a '\r' or '\n'
  while (!complete) {
    if (U3STAbits.URXDA) { // if data is available
      data = U3RXREG;      // read the data
      if ((data == '\n') || (data == '\r')) {
        complete = 1;
      } else {
        message[num_bytes] = data;
        ++num_bytes;
        // roll over if the array is too small
        if (num_bytes >= maxLength) {
          num_bytes = 0;
        }
      }
    }
  }
  // end the string
  message[num_bytes] = '\0';
}

// Write a character array using UART3
void NU32_WriteUART3(const char * string) {
  while (*string != '\0') {
    while (U3STAbits.UTXBF) {
      ; // wait until tx buffer isn't full
    }
    U3TXREG = *string;
    ++string;
  }
}
