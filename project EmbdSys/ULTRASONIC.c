#include "ULTRASONIC.h"


volatile uint32_t echoStart = 0;
volatile uint32_t echoWidth = 0;
volatile bool echoReady = false;

void GPIOL_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 10);
	while((SYSCTL -> PRGPIO & (1 << 10)) == 0);
	
	GPIOL -> DIR |= (1 << 5);
	GPIOL -> DIR &= ~(1 << 4);
	GPIOL -> AFSEL |= (1 << 4);
	GPIOL -> AFSEL &= ~(1 << 5);
	GPIOL -> PCTL &= ~(0xF << 16);                    
	GPIOL -> PCTL |= (3 << 16);                      
	GPIOL -> DEN |= (1 << 4) | (1 << 5);
	GPIOL -> DATA &= ~ (1 << 5);
}

void TIMER0_setup(void)
{
	GPIOL_setup();
	
	SYSCTL -> RCGCTIMER |= (1 << 0);           
	while((SYSCTL -> PRTIMER & (1 << 0)) == 0);
	
	TIMER0 -> CTL = 0;
	TIMER0 -> CFG = (1 << 2);
	TIMER0 -> TAMR = (3 << 0) | (1 << 2) | (1 << 4);
	TIMER0 -> CTL = (3 << 2);
	TIMER0 -> TAILR = (0xFFFF << 0);
	TIMER0 -> TAPR = (0xFF << 0);
	
	TIMER0 -> ICR = (1 << 2);
	TIMER0 -> IMR |= (1 << 2); 
	NVIC -> ISER[0] |= (1 << 19);
	
	TIMER0 -> CTL |= (1 << 0);
}

void initULTRASONIC(void)				{			TIMER0_setup();		}

void ultrasonic_trigger(void)		{		GPIOL -> DATA |= (1 << 5);			ES_usDelay(150);				GPIOL -> DATA &= ~(1 << 5);  }

void TIMER0A_Handler(void)
{
	uint32_t now;
	
	TIMER0 -> ICR = (1 << 2);                     
	now = TIMER0 -> TAR & (0xFFFFFF << 0);               
	
	if		(GPIOL -> DATA & (1 << 4))	{		echoStart = now;																														}
	else 		         		              { 	echoWidth = (now - echoStart) & (0xFFFFFF << 0);  			echoReady = true;		}
}

uint32_t ultrasonic_getCM(void)	{		return echoWidth / 928;		} // 16 counts/us x 58 us/cm