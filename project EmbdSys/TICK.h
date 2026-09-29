#ifndef TICK_H
#define TICK_H

#include "INCLUDES.h"

extern volatile uint32_t msTick;


void SYSTICK_setup(void);

void initTICK(void);

void SysTick_Handler(void);

#endif