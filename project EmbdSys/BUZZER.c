#include "BUZZER.h"

void GPIOG_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 6);
	while((SYSCTL -> PRGPIO & (1 << 6)) == 0);
	
	GPIOG_AHB -> AFSEL |= (1 << 0);
	GPIOG_AHB -> PCTL &= ~0xF;
	GPIOG_AHB -> PCTL |= 0x6;
	GPIOG_AHB -> DEN |= (1 << 0);
}

void PWM0_setup(void)
{
	GPIOG_setup();
	
	SYSCTL -> RCGCPWM |= (1 << 0);
	while((SYSCTL -> PRPWM & (1 << 0)) == 0);
	
	PWM0 -> CC = 0x102;
}

void initBUZZER(void)
{
	PWM0_setup();
	
	PWM0 -> _2_CTL = 0x00;
	PWM0 -> _2_GENA = 0x8C;
	PWM0 -> _2_LOAD = 999;
	PWM0 -> _2_CMPA = 500;
	PWM0 -> _2_CTL = 0x01; 
	
}

void buzzer_on(void) { PWM0 -> ENABLE |= (1 << 4);	 }
void buzzer_off(void){ PWM0 -> ENABLE &= ~(1 << 4);	 }

void buzzer_beep(uint32_t ms)
{
	buzzer_on();
	ES_msDelay(ms);
	buzzer_off();
}