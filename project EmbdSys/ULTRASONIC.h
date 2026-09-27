#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "ES.h"

extern volatile uint32_t echoStart;
extern volatile uint32_t echoWidth;
extern volatile bool echoReady;

void GPIOL_setup(void);

void TIMER0_setup(void);

void initULTRASONIC(void);

void ultrasonic_trigger(void);

void TIMER0A_Handler(void);

uint32_t ultrasonic_getCM(void); //getter function

#endif