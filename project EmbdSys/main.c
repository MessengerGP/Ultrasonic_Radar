#include "ES.h"
#include "UART.h"
#include "RADAR.h"
#include "LCD.h"
#include "LCD_Display.h"
	

//run at 15 micro seconds not 10
// -----------------Main function-------------------
int main(void)
{
		
	UART_setup();
	initSPI();
	initLCD();
	fillScreen(ILI9341_RED);

	__enable_irq();
	 
	
while(true)
{
    if(hasChar)
    {
        hasChar = false;

        if(holdChar == 27){
            ES_printf(0, "aESC was entered.\nState received: IDLE\n\n--------------------------\n\n");
        }
        else if(holdChar == 'a' || holdChar == 'A'){
            ES_printf(0, " was entered.\nState received: AUTOMATIC\n\n--------------------------\n\n");
        }
        else if(holdChar == 'm' || holdChar == 'M'){
            ES_printf(0, " was entered.\nState received: MANUAL\n\n--------------------------\n\n");
        }

        change_state(holdChar);
        set_bounds(holdChar);
    }
}
	
  return 0;
}


