#include "ES.h"
#include "UART.h"
#include "RADAR.h"
#include "LCD.h"
#include "LCD_Display.h"
#include "BUZZER.h"
#include "POT.h"
#include "SERVO.h"
#include "ULTRASONIC.h"
#include "CONTROLLER.h"
#include "TICK.h"


int main(void)
{
	ES_setSystemClockFrequency(16); 
	ES_startDelayTimer();
		
	initUART();
	initBUZZER();
	initADC();
	initSERVO();
	initULTRASONIC();
	initCONTROLLER();
	initTICK();
	
	initSPI();
	initLCD();

	__enable_irq();
	 
	while(true)
	{
    if(hasButtonPressed)
    {
			hasButtonPressed = false;

        if(holdingLastPressed == 27){																				
					ES_printf(0, "ESC was entered.\nState received: IDLE\n\n--------------------------\n\n"); 
					buzzer_beep(150);
				}
        else if (holdingLastPressed == 'a' || holdingLastPressed == 'A'){		ES_printf(0, " was entered.\nState received: AUTOMATIC\n\n--------------------------\n\n");		}
        else if (holdingLastPressed == 'm' || holdingLastPressed == 'M'){		ES_printf(0, " was entered.\nState received: MANUAL\n\n--------------------------\n\n");			}
				
				else if (holdingLastPressed == 'p' || holdingLastPressed == 'P'){
					ADC_start();
					while(!adcReady);                              
					adcReady = false;
					ES_printf(0, "\nPot: %d   X: %d   Y: %d\n", potValue, joyX, joyY); 		
					servo_setAngle(potValue * 180 / 4095);        
				}
				
				else if (holdingLastPressed == 'd' || holdingLastPressed == 'D'){
					echoReady = false;
					ultrasonic_trigger();
					ES_msDelay(60);                                 									
    
					if(echoReady)	{  ES_printf(0, "\nDistance: %d cm\n\n--------------------------\n\n", ultrasonic_getCM());	}
					else 					{	 ES_printf(0, "\nNo echo\n");																															}
				}
				
				else if(holdingLastPressed == 't' || holdingLastPressed == 'T'){    ES_printf(0, "\nTicks: %d ms\n", msTick);			}

			change_state(holdingLastPressed);
			set_bounds(holdingLastPressed);
    }
		
		if(state == MANUAL)			{		manual_run();	}
		if(state == AUTOMATIC)	{		auto_run();		}
	}
	
  return 0;
}


