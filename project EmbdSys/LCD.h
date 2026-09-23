#ifndef LCD_H
#define LCD_H

#include "ES.h"

void GPIOD_setup(void);
void initSPI(void);
void spi_Transmit(uint8_t data);

#endif