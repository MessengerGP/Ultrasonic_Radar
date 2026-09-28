#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ES.h"
#include "UART.h" // for the button being pressedb

	
void GPIOM_setup(void); // bit 11

void initCONTROLLER(void);

void GPIOM_Handler(void);


#endif