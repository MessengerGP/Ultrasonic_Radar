#ifndef LCD_H
#define LCD_H

#include "INCLUDES.h"

void GPIOD_setup(void);
void initSPI(void);

void lcd_modeInfo(void);
void lcd_liveInfo(int angle);
void lcd_error(char *reason, int x);

void lcd_sonarBase(void);
void lcd_sonarLine(int servoAngle);
void lcd_sonarMap(int servoAngle);
void lcd_echoDot(int servoAngle, int cm);
void lcd_gauge(int angle);

#endif