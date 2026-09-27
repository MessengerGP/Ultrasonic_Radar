#ifndef UART_H
#define UART_H

#include "ES.h"

extern volatile char holdingLastPressed;
extern volatile bool hasButtonPressed; 

void GPIOA_setup(void);

void initUART(void);

void UART_sendChar(char c);

void UART_sendString(char *str);

void UART0_Handler(void);

#endif