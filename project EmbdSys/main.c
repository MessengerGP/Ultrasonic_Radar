#include "ES.h"
#include "UART.h"
#include "RADAR.h"
#include "LCD.h"
#include "LCD_Display.h"
#include "BUZZER.h"
#include "POT.h"
#include "SERVO.h"
#include "ULTRASONIC.h"

//run at 15 micro seconds not 10
// -----------------Main function-------------------
int main(void)
{
	ES_setSystemClockFrequency(16); 
	ES_startDelayTimer();
		
	initUART();
	initBUZZER();
	initADC();
	initSERVO();
	initULTRASONIC();
	initSPI();
	initLCD();

	__enable_irq();
	 
	
while(true)
{
    if(hasChar)
    {
        hasChar = false;

        if(holdChar == 27){
            ES_printf(0, "ESC was entered.\nState received: IDLE\n\n--------------------------\n\n");
						buzzer_beep(150);
        }
        else if(holdChar == 'a' || holdChar == 'A'){
            ES_printf(0, " was entered.\nState received: AUTOMATIC\n\n--------------------------\n\n");
        }
        else if(holdChar == 'm' || holdChar == 'M'){
            ES_printf(0, " was entered.\nState received: MANUAL\n\n--------------------------\n\n");
        }
				else if(holdChar == 'p' || holdChar == 'P'){
						ADC_start();
						while(!adcReady);                               // wait for ADC0SS2_Handler
						adcReady = false;
						ES_printf(0, "\nPot: %d   Angle: %d\n", potValue, potValue * 180 / 4095);
					  servo_setAngle(potValue * 180 / 4095);          // move servo to pot angle
				}
				else if(holdChar == 'd' || holdChar == 'D'){
					echoReady = false;
					ultrasonic_trigger();
					ES_msDelay(60);                                 // give the echo time to return
    
					if(echoReady){
						ES_printf(0, "\nDistance: %d cm\n", ultrasonic_getCM());
					}
					else{
					ES_printf(0, "\nNo echo\n");
					}
				}

        change_state(holdChar);
        set_bounds(holdChar);
    }
}
	
  return 0;
}


