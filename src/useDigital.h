#ifndef useDigital_H
#define useDigital_H

//includes

//defines
extern volatile int manual_auto; //manual = 1, auto = 0

//functions
void initDigital(void);						//Initializes the digital function
int getManualAuto(void);
void invManualAuto(void);

#endif