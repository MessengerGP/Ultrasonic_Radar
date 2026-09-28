#include "POT.h"

volatile uint16_t potValue = 0;
volatile uint16_t joyX = 0;					//AIN1, PE2 
volatile uint16_t joyY = 0;					//AIN2, PE1
volatile bool adcReady = false;

void GPIOE_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 4); 
	while((SYSCTL -> PRGPIO & (1 << 4)) == 0);
	
	//CHANGED: PE1 (Y), PE2 (X), PE3 (pot) all analog inputs
	GPIOE_AHB -> AMSEL |= (1 << 1) | (1 << 2) | (1 << 3);       
	GPIOE_AHB -> DIR &= ~((1 << 1) | (1 << 2) | (1 << 3));  
	GPIOE_AHB -> AFSEL |= (1 << 1) | (1 << 2) | (1 << 3);  
	GPIOE_AHB -> DEN &= ~((1 << 1) | (1 << 2) | (1 << 3));      
}

void ADC_setup(void)
{
	GPIOE_setup();
	
	SYSCTL -> RCGCADC |= (1 << 0);                  
	while((SYSCTL -> PRADC & (1 << 0)) == 0);       
	
	ADC0 -> CC = (1 << 0);															// ADC uses the 16 MHz internal clock

}

void initADC(void)
{
	ADC_setup();
	
	ADC0 -> ACTSS &= ~(1 << 2);
	ADC0 -> EMUX &= ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
	
	//new with joystick
	ADC0 -> SSMUX2 = (0 << 0) | (1 << 4) | (2 << 8);		// read pot, then joystick X, then joystick Y
	ADC0 -> SSCTL2 = (1 << 9) | (1 << 10);							//END (bit 9) + interrupt (bit 10)
	
	ADC0 -> ISC = (1 << 2);                         		// clear any old interrupt
	ADC0 -> IM |= (1 << 2);                         		// allow the interrupt
	NVIC -> ISER[0] |= (1 << 16);                   		// turn on ADC interrupt (IRQ 16)
	
	ADC0 -> ACTSS |= (1 << 2);
}

void ADC_start(void)	{	 ADC0 -> PSSI = (1 << 2);	}		// take one set of readings (pot, X, Y)

void ADC0SS2_Handler(void)
{
	// results come out in the same order they were read
	potValue = ADC0 -> SSFIFO2 & 0xFFF;             // pot        (PE3)
	joyX     = ADC0 -> SSFIFO2 & 0xFFF;             // joystick X (PE2)
	joyY     = ADC0 -> SSFIFO2 & 0xFFF;             // joystick Y (PE1)
	
	adcReady = true;                               
	ADC0 -> ISC = (1 << 2);     
}
