#include "CONTROLLER.h"
#include "TICK.h"		//uses msTick

#define BUTTON_WAIT 200

static uint32_t lastPressTime = 0;


void GPIOM_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 11);
	while((SYSCTL -> PRGPIO & (1 << 11)) == 0);
	
	GPIOM -> DIR &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
	GPIOM -> DEN |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
	GPIOM -> PUR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
}

void initCONTROLLER(void)
{
	GPIOM_setup();
	
	GPIOM -> IS &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
	GPIOM -> IBE &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
	GPIOM -> IEV &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
	GPIOM -> ICR = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
	GPIOM -> IM |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
	
	NVIC -> ISER[2] |= (1 << 8);
}

void GPIOM_Handler(void)
{
	bool pressed = false;
	
	if((msTick - lastPressTime) >= BUTTON_WAIT)
	{
		if			(GPIOM -> MIS & (1 << 0))		{		holdingLastPressed 	= 'a'; 		pressed = true;		} 	// 	AUTOMATIC 		Button A 	or 	Key 	A
		else if	(GPIOM -> MIS & (1 << 1))		{		holdingLastPressed 	=	'<';    pressed = true;		}		// 	MIN ANGLE 		Button B 	or 	Key 	<
		else if	(GPIOM -> MIS & (1 << 2))		{		holdingLastPressed 	= 'm';   	pressed = true;		}		//	MANUAL				Button C 	or 	Key 	M
		else if	(GPIOM -> MIS & (1 << 3))		{		holdingLastPressed 	= '>';   	pressed = true;		}		// 	MAX ANGLE			Button D 	or 	Key 	>
		else if	(GPIOM -> MIS & (1 << 4))		{		holdingLastPressed 	= 'j';    pressed = true;		}		//	SWITCH				Button E 	or 	Key 	J
		else if	(GPIOM -> MIS & (1 << 5))		{		holdingLastPressed 	= 'd';    pressed = true;		} 	// 	ECHO					Button F 	or 	Key 	D
		else if	(GPIOM -> MIS & (1 << 6))		{		holdingLastPressed 	= 27;   	pressed = true;		} 	//	IDLE					Joystick  or 	Key 	ESC
		
		if(pressed){
			lastPressTime = msTick; 
			hasButtonPressed = true;            
			if(holdingLastPressed != 27){		UART_sendChar(holdingLastPressed);		}	
		}
	}
	
	// clear all Port M flags
	GPIOM -> ICR = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);     
}