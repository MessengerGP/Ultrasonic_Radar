#ifndef BUZZER_H
#define BUZZER_H

#include "INCLUDES.h"


void GPIOG_setup(void);
void PWM0_setup(void);

void initBUZZER(void);

void buzzer_on(void);
void buzzer_off(void);

void buzzer_beep(uint32_t ms);

#endif