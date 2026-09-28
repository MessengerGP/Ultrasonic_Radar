#ifndef POT_H
#define POT_H

#include "ES.h"

extern volatile uint16_t potValue;
extern volatile uint16_t joyX;                      
extern volatile uint16_t joyY;                     
extern volatile bool adcReady;

void GPIOE_setup(void);
void ADC_setup(void);

void initADC(void);
void ADC_start(void);

void ADC0SS2_Handler(void);

#endif