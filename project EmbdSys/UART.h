#ifndef UART_H
#define UART_H

#include "ES.h"

extern volatile char holdChar;
extern volatile bool hasChar; 

void GPIOA_setup(void);

void UART_setup(void);

void UART_sendChar(char c);

void UART_sendString(char *str);

void UART0_Handler(void);

#endif