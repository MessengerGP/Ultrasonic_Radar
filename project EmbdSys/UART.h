#ifndef UART_H
#define UART_H

#include "INCLUDES.h"

void GPIOA_setup(void);

void initUART(void);

void UART_sendChar(char c);

void UART_sendString(char *str);

void UART0_Handler(void);

#endif