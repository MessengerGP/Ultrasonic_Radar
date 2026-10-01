#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "INCLUDES.h"

extern volatile uint32_t echoStart;
extern volatile uint32_t echoWidth;
extern volatile bool echoReady;
extern char echoText[10];

void GPIOL_setup(void);

void TIMER0_setup(void);

void initULTRASONIC(void);

void ultrasonic_trigger(void);

void TIMER0A_Handler(void);

uint32_t ultrasonic_getCM(void);	// getter function - returns the last measured distance in cm

#endif