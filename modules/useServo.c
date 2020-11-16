//todo

//includes

//defines

//functions
void initServo(void)						//Initializes the servo IO & PWM
{
	//////Servo setup
  int angle;
  float DC;
  T3CONbits.TCKPS = 0b110;     // Timer3 prescaler N=4 (1:4)
  PR3 = 24999;              // 20 ms
  TMR3 = 0;                // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC1CONbits.OCTSEL = 1;    // optional (default)
  OC1RS = 625;             // duty cycle = OC1RS/(PR2+1) = 25%
  OC1R = 625;              // initialize before turning OC1 on; afterward it is read-only
  T3CONbits.ON = 1;        // turn on Timer2
  OC1CONbits.ON = 1;       // turn on OC1
/////End servo setup
}