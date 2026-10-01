#include "INCLUDES.h"

/*
--- WIRE COLOURS ------------------- USAGE --------------------  EXTRA? ------------
YELLOW 							= 	(+3.3V, +5V) and (GND) 						&		 (7)		 	(+1 jumper)
BLACK 							= 	(BUZZER) 													&		 (2)		 	(+1 jumper)
GREY & WHITE 				= 	(SERVO) and (RADAR)								&		 (6)(4)		(N/A)
BLUE 								= 	(POTENTIOMETER)										&		 (3)		 	(+1 jumper)
RED & ORANGE 				= 	(LCD)															&		 (5)(7)		(+1 jumper)
REMAINING WIRES 		= 	(FUNDUINO JOYSTICK SHIELD V1.A)		&		 (11)		 	(N/A)
------------------------------------------------------------------------------------

----- BREADBOARD ------------------- USAGE ---------------------	WHERE? -----------
100 ohms					 	=		BUZZER current limit							&			C10 to C13
1k 	ohms						=		ECHO voltage divider							&			H32 to H35
2k 	ohms						=		ECHO voltage divider							&			I35 to -rail (blue) 
------------------------------------------------------------------------------------
*/



int main(void)
{
		char text[10] = "";
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
// ----------------------------------
	
	setRotation(3);
	
	fillScreen(black);
	
	lcd_modeInfo();

// ----------------------------------
	
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
				
				setRotation(2);
				setCharConfig(white, 1, 1, black, 1);
				moveCursor(11, 100);			drawString("ECHO:", 5);
				moveCursor(115, 100);			drawString("cm", 2);
				
				setCharConfig(black, 1, 1, black, 1);
				moveCursor(75, 100);			drawString(text, strlen(text));
				moveCursor(75, 100);			drawString("N/A", 3);
				setCharConfig(white, 1, 1, black, 1);
				
				if(echoReady && ultrasonic_getCM() <= 200)
				{
					ES_printf(0, "\nDistance: %d cm\n\n--------------------------\n\n", ultrasonic_getCM());
					sprintf(text, "%03d", ultrasonic_getCM());
					moveCursor(75, 100);			drawString(text, strlen(text));
				}
				else
				{
					ES_printf(0, "\nNo echo\n");
					moveCursor(75, 100);			drawString("N/A", 3);
					lcd_error("no echo", 49);
				}
				setRotation(3);
			}
			
			else if(holdingLastPressed == 't' || holdingLastPressed == 'T'){    ES_printf(0, "\nTicks: %d ms\n", msTick);			}

			change_state(holdingLastPressed);
			set_bounds(holdingLastPressed);
			lcd_modeInfo();
		}
		
		if(state == MANUAL)			{		manual_run();	}
		if(state == AUTOMATIC)	{		auto_run();		}
	}
	
  return 0;
}


