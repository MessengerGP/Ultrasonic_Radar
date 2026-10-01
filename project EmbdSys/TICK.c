#include "INCLUDES.h"

void SYSTICK_setup(void)
{
	SysTick -> CTRL = 0;
	SysTick -> LOAD = 15999;
	SysTick -> VAL = 0;
	SysTick -> CTRL |= (1 << 0) | (1 << 1) | (1 << 2);
}

void initTICK(void)					{		SYSTICK_setup();	}
void SysTick_Handler(void)	{		msTick++;					}