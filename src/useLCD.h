#ifndef USE_LCD_H
#define USE_LCD_H
// to use the LCD for Good Morning Blinds Project
//todo


//defines
#define MSG_LEN_UPPER 16			//Upper row is 16 char long
#define MSG_LEN_LOWER 16			//Lower row is 16 char long
extern volatile int day_night;  /* Declaration of the variable */

//functions
void initLCD(void);
void timeStatusLCD(int state);
void lightLevelLCD(int lightLvl);
void servoPosLCD(int servoPos);
void batteryLvlLCD(int battLvl);

#endif
