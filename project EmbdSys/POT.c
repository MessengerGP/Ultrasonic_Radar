#include "POT.h"

volatile uint16_t potValue = 0;
volatile bool adcReady = false;

void GPIOE_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 4);                 // Port E clock
	while((SYSCTL -> PRGPIO & (1 << 4)) == 0);      // wait until ready
	
	GPIOE_AHB -> AMSEL |= (1 << 3);                 // PE3 analog on
	GPIOE_AHB -> DIR &= ~(1 << 3);                  // PE3 input
	GPIOE_AHB -> AFSEL |= (1 << 3);                 // alternate function
	GPIOE_AHB -> DEN &= ~(1 << 3);                  // digital off
}

void ADC_setup(void)
{
	GPIOE_setup();
	
	SYSCTL -> RCGCADC |= (1 << 0);                  // ADC0 clock
	while((SYSCTL -> PRADC & (1 << 0)) == 0);       // wait for ADC0 (was PRGPIO)
	
	ADC0 -> CC = 0x1;                               // ADC clock from 16 MHz PIOSC
}

void initADC(void)
{
	ADC_setup();
	
	ADC0 -> ACTSS &= ~(1 << 2);                     // disable SS2 while configuring
	ADC0 -> EMUX &= ~0xF00;                         // SS2 trigger = processor (software)
	ADC0 -> SSMUX2 = 0;                             // sample 0 = AIN0 (PE3, pot)
	ADC0 -> SSCTL2 = 0x6;                           // sample 0: IE0 + END0
	
	ADC0 -> ISC = (1 << 2);                         // clear old SS2 flag
	ADC0 -> IM |= (1 << 2);                         // unmask SS2 interrupt
	NVIC -> ISER[0] |= (1 << 16);                   // enable IRQ 16 (ADC0 SS2)
	
	ADC0 -> ACTSS |= (1 << 2);                      // enable SS2
}


void ADC_start(void){	 ADC0 -> PSSI = (1 << 2);	} // start one SS2 conversion


void ADC0SS2_Handler(void) // SS2: up to 4 samples, FIFO = 4. USING 4 TO ADD JOYSTICK X\Y LATER
{
	potValue = ADC0 -> SSFIFO2 & 0xFFF;             // 12-bit result
	adcReady = true;                                // tell main a reading is ready
	ADC0 -> ISC = (1 << 2);                         // clear SS2 interrupt
}

//AIN0 = PE3
