#ifndef LCD_H
#define LCD_H
// to use the LCD for Good Morning Blinds Project
//todo

//defines

//functions
void initLCD(void);
void timeStatusLCD(int state);
void lightLevelLCD(int lightLvl);
void servoPosLCD(int servoPos);
void batteryLvlLCD(int battLvl);

#endif
