#ifndef LCD_H
#define LCD_H

#include "INCLUDES.h"

void GPIOD_setup(void);
void initSPI(void);


void lcd_modeInfo(void);
void lcd_liveInfo(int angle);
void lcd_error(char *reason, int x);
#endif