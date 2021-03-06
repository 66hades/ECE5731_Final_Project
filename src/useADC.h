#ifndef LCD_H
#define LCD_H
// to use the ADC inputs for Good Morning Blinds Project

//includes
#include "NU32.h"          // constants, functions for startup and UART
#include <stdio.h>
#include "useServo.h"
#include "useDigital.h"
//defines
extern volatile int lightPerc; /* Declaration of the variable */
extern volatile int battPerc;  /* Declaration of the variable */
extern volatile int anglePos;  /* Declaration of the variable */

//functions
unsigned int adc_sample_convert(int pin);
void initADC(void);
void setPhoto(void);
void setBatt(void);
void setPot(void);
int getPhoto(void);
int getPot(void);
int getBatt(void);


#endif