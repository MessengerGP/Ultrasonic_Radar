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
		if(GPIOM -> MIS & (1 << 0)){                  // button A or Key A
			holdingLastPressed = 'a';                   // AUTOMATIC
				pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 1)){             // button B or Key <
			holdingLastPressed = '<';                   // min angle
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 2)){             // button C or Key M
			holdingLastPressed = 'm';                   // MANUAL
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 3)){             // button D or Key >
			holdingLastPressed = '>';                   // max angle
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 6)){             // joystick button or Key ESC
			holdingLastPressed = 27;                    // IDLE
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 4)){             // button E or Key J
			holdingLastPressed = 'j';                   // switch
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 5)){             // button F or Key D
			holdingLastPressed = 'd';                   // echo
			pressed = true;
		}
	
		if(pressed){
			lastPressTime = msTick; 
			hasButtonPressed = true;                    // hand it to main
		
			if(holdingLastPressed != 27){		UART_sendChar(holdingLastPressed);		}	// echo like the keyboard, except ESC
		}
	}
	
	// clear all Port M flags
	GPIOM -> ICR = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);     
}