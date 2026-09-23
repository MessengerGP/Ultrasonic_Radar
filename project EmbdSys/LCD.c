#include "LCD.h"


void GPIOD_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 3);
	while((SYSCTL -> PRGPIO & (1 << 3)) == 0);
	
	GPIOD_AHB -> DIR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> DEN |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> PUR |= (1 << 3);
	GPIOD_AHB -> AFSEL |= (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> PCTL &= ~0xFFF0;
	GPIOD_AHB -> PCTL |= 0xFFF0;
}


void initSPI(void)
{
	GPIOD_setup();
	
	SYSCTL -> RCGCSSI |= (1 << 2);
	while((SYSCTL -> PRSSI & (1 << 2)) == 0);
	
	SSI2 -> CR1 &= ~((1 << 1) | (1 << 2)); //disable SSI, ms to master
	
	SSI2 -> CPSR = 4; //CPSDVSR=4 -> baud = 16MHz / 4 = 4MHz

	SSI2 -> CR0 = 0xC7; // SRC=0, SPH=1, SP0=1, FRF=00 (Freescale), DDS=0x7 (8bit)
	//SRC, spi mode, desired clock freq/polarity (SPH and SPO), protocol mode (FRF), data size (DSS)

	
	SSI2 -> CR1 |= (1 << 1); //enable SSI
}


void spi_Transmit(uint8_t data)
{
	while(!(SSI2 -> SR & (1 << 1)));
	SSI2 -> DR = data;
}