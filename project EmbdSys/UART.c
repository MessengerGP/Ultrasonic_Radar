#include "INCLUDES.h"

volatile char holdingLastPressed;
volatile bool hasButtonPressed = false; 

void GPIOA_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 0);
	while((SYSCTL -> PRGPIO & (1 << 0)) == 0);
	
	GPIOA_AHB -> AFSEL |= (1 << 0) | (1 << 1);
	GPIOA_AHB -> PCTL &= ~((0xF << 0) | (0xF << 4));
	GPIOA_AHB -> PCTL |= (1 << 0) | (1 << 4);
	GPIOA_AHB -> AMSEL &= ~((1 << 0) | (1 << 1));
	GPIOA_AHB -> DEN |= (1 << 0) | (1 << 1);
}

void initUART(void)
{
	GPIOA_setup();
	
	SYSCTL -> RCGCUART |= (1 << 0);
	while((SYSCTL -> PRUART & (1 << 0)) == 0);
	
	UART0 -> CTL &= ~((1 << 0) | (1 << 8) | (1 << 9));
	UART0 -> IBRD = 8;
	UART0 -> FBRD = 44;
	UART0 -> LCRH &= ~(0xFF << 0);
	UART0 -> LCRH |= (3 << 5);
	UART0 -> CC = 0;
	UART0 -> IM |= (1 << 4);                                                                                        
	
	NVIC -> ISER[0] |= (1 << 5);
	UART0 -> CTL |= (1 << 0) | (1 << 8) | (1 << 9);
}

void UART_sendChar(char c)
{
	while(UART0 -> FR & (1 << 5));
	UART0 -> DR = c;
}

void UART_sendString(char *str)
{
	int length = strlen(str);
	for	(int i = 0; i < length; i++)	{	UART_sendChar(str[i]);	}
}

void UART0_Handler(void)
{
	if(UART0 -> MIS & (1 << 4)){
		holdingLastPressed = UART0 -> DR & (0xFF << 0);
		hasButtonPressed = true;
		
		if(holdingLastPressed != 27)		{ UART_sendChar(holdingLastPressed);	}

		UART0 -> ICR = (1 << 4);
	}
}