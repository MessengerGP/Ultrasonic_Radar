#ifndef LCD_H
#define LCD_H

#include "INCLUDES.h"

#define RADAR_X     160
#define RADAR_Y     235
#define RADAR_R     180
#define RING_GAP    50
#define PI          3.14159



void GPIOD_setup(void);
void initSPI(void);

void lcd_radarBackground(void);

#endif