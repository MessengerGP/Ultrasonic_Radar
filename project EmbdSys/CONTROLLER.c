#include "CONTROLLER.h"
#include "TICK.h"		//uses msTick


#define BUTTON_WAIT 200

static uint32_t lastPressTime = 0;


void GPIOM_setup(void) // bit 11
{
	// Port M 0-6 (7 total)
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
		if(GPIOM -> MIS & (1 << 0)){                  // button A
			holdingLastPressed = 'a';                   // AUTOMATIC
				pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 1)){             // button B
			holdingLastPressed = '<';                   // min angle
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 2)){             // button C
			holdingLastPressed = 'm';                   // MANUAL
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 3)){             // button D
			holdingLastPressed = '>';                   // max angle
			pressed = true;
		}
		else if(GPIOM -> MIS & (1 << 6)){             // joystick button 
			holdingLastPressed = 27;                    // ESC
			pressed = true;
		}
	
		// PM4 (E) not being used
	
		// PM5 (F) not being used
	
		if(pressed){
			lastPressTime = msTick; 
			hasButtonPressed = true;                    // hand it to main
		
			if(holdingLastPressed != 27){    // echo like the keyboard, except ESC
				UART_sendChar(holdingLastPressed);
			}
		}
	}
	
	// clear all Port M flags
	GPIOM -> ICR = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);     
}