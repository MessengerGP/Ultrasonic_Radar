#include "LCD.h"


void GPIOD_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 3);
	while((SYSCTL -> PRGPIO & (1 << 3)) == 0);
	
	GPIOD_AHB -> DIR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> DEN |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> PUR |= (1 << 3);
	GPIOD_AHB -> AFSEL |= (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> PCTL &= ~((0xF << 4) | (0xF << 8) | (0xF << 12));
	GPIOD_AHB -> PCTL |= (0xF << 4) | (0xF << 8) | (0xF << 12);
}


void initSPI(void)
{
	GPIOD_setup();
	
	SYSCTL -> RCGCSSI |= (1 << 2);
	while((SYSCTL -> PRSSI & (1 << 2)) == 0);
	
	SSI2 -> CR1 &= ~((1 << 1) | (1 << 2)); 
	SSI2 -> CPSR = 4; 												
	SSI2 -> CR0 = (1 << 7) | (1 << 6) | (7 << 0);										
	SSI2 -> CR1 |= (1 << 1); 
}


void spi_Transmit(uint8_t data)
{
	while(!(SSI2 -> SR & (1 << 1)));
	SSI2 -> DR = data;
}