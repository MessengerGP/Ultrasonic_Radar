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
	lcd_sonarBase();

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

			
			else if (holdingLastPressed == 'd' || holdingLastPressed == 'D'){
				echoReady = false;
				ultrasonic_trigger();
				ES_msDelay(60);
				
				setRotation(2);
				setCharConfig(black, 1, 1, black, 1);
				moveCursor(75, 100);			drawString(echoText, strlen(echoText));
				setRotation(3);
				
				if(echoReady && ultrasonic_getCM() <= 200)
				{
					ES_printf(0, "\nDistance: %d cm\n\n--------------------------\n\n", ultrasonic_getCM());
					sprintf(echoText, "%03d", ultrasonic_getCM());
					lcd_echoDot(radar_getAngle(), ultrasonic_getCM());
					buzzer_beep(50);
				}
				else
				{
					ES_printf(0, "\nNo echo\n");
					strcpy(echoText, "N/A");
					lcd_echoDot(radar_getAngle(), -1);
					lcd_error("no echo", 49);
				}
			}

			change_state(holdingLastPressed);
			set_bounds(holdingLastPressed);
			lcd_modeInfo();
		}
		
		if(state == MANUAL)			{		manual_run();	}
		if(state == AUTOMATIC)	{		auto_run();		}
	}
	
  return 0;
}


