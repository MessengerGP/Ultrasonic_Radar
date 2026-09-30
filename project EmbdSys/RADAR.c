#include "INCLUDES.h"


int state = IDLE;
int minAngle = 0;
int maxAngle = 180;

bool joystickCTRL = false;
static int servoPOS = 90;			// to remember angle when swapping back and forth

static bool directionPOS = false;	// false = first sweep down to min		|		 true = first sweep up to max


void change_state(char c)
{
	if			(c == 27)									{		idle_state();		}	
	else if	(c == 'a' || c == 'A')		{		auto_state();		}
	else if	(c == 'm' || c == 'M')		{		manual_state();	}
	else if	(c == 'j' || c == 'J')
	{
		if(!joystickCTRL)								{		joystickCTRL = true;				ES_printf(0, "\nControl: JOYSTICK\n");					}
		else														{		joystickCTRL = false;				ES_printf(0, "\nControl: POTENTIOMETER\n");			}
	}
}


void set_bounds(char key)
{
	if(state == IDLE)
	{
		if			(key == 60)								{		minAngle += 10;
			if		(minAngle > 180)					{		minAngle = 0;																																																																	}
			if		(minAngle >= maxAngle)		{		ES_printf(0, "\rERROR! MIN ANGLE LARGER THAN MAX ANGLE!\n\n--------------------------\n\n");				buzzer_beep(500);			minAngle = 0;				}
			else														{		ES_printf(0, "MIN ANGLE RANGE: %d\nMAX ANGLE RANGE: %d\n\n--------------------------\n\n", minAngle, maxAngle);																}
		}
		else if	(key == 62)								{		maxAngle -= 10;
			if		(maxAngle < 0)						{		maxAngle = 180;																																																																}
			if		(maxAngle <= minAngle)		{		ES_printf(0, "\rERROR! MAX ANGLE SMALLER THAN MIN ANGLE!\n\n--------------------------\n\n");				buzzer_beep(500);			maxAngle = 180;			}
			else														{		ES_printf(0, "MIN ANGLE RANGE: %d\nMAX ANGLE RANGE: %d\n\n--------------------------\n\n", minAngle, maxAngle);																}
		}
	}
}


void idle_state	  (void)		{		state = IDLE;				fillRect(0, 0, 159, 240, black);		fillRect(159, 0, 3, 240, green);		}
void auto_state	  (void)		{		state = AUTOMATIC;	fillRect(0, 0, 159, 240, black);		fillRect(159, 0, 3, 240, green);		}
void manual_state (void)		{		state = MANUAL;			fillRect(0, 0, 159, 240, black);		fillRect(159, 0, 3, 240, green);		}


void manual_run(void)
{
	int lastPOS = servoPOS;
	
	ADC_start();
	while(!adcReady);
	adcReady = false;
	
	if(!joystickCTRL)
	{
		servoPOS = potValue * 180 / 4095;
		servo_setAngle(servoPOS);
	
		echoReady = false;
		ultrasonic_trigger();
		ES_msDelay(60);
	
		if(echoReady){
			ES_printf(0, "\rPot Angle: %3d Degrees  Distance: %3d cm		", servoPOS, ultrasonic_getCM());
		}
		else{
			ES_printf(0, "\rPot Angle: %3d Degrees  Distance: ------		", servoPOS);
		}
		
		if(!joystickCTRL)
		{
			servoPOS = potValue * 180 / 4095;
			servo_setAngle(servoPOS);
	
			echoReady = false;
			ultrasonic_trigger();
			ES_msDelay(60);
	
			if(echoReady) {		ES_printf(0, "\rPot Angle: %3d Degrees  Distance: %3d cm		", servoPOS, ultrasonic_getCM());		}
			else					{		ES_printf(0, "\rPot Angle: %3d Degrees  Distance: ------		", servoPOS);												}
			lcd_liveInfo(servoPOS);
			if((servoPOS == 0 || servoPOS == 180) && servoPOS != lastPOS)		{		buzzer_beep(50);		ES_msDelay(50);	 	 buzzer_beep(50);		}
		}
	}
	
	else
	{
		if			(joyX > 2400)		{		servoPOS = servoPOS + 8;		}		
		else if	(joyX < 1700)		{		servoPOS = servoPOS - 8;		}	
		
		if	(servoPOS > 180)		{		servoPOS = 180;		}						
		if	(servoPOS < 0)			{		servoPOS = 0;			}
		
		servo_setAngle(servoPOS);
	
		echoReady = false;
		ultrasonic_trigger();
		ES_msDelay(60);
	
		if(echoReady)	{		ES_printf(0, "\rjoystick Angle: %3d Degrees  Distance: %3d cm		", servoPOS, ultrasonic_getCM());		}
		else					{		ES_printf(0, "\rjoystick Angle: %3d Degrees  Distance: ------		", servoPOS);												}
		lcd_liveInfo(servoPOS);
		if((servoPOS == 0 || servoPOS == 180) && servoPOS != lastPOS)		{		buzzer_beep(50);		ES_msDelay(50);	 	 buzzer_beep(50);		}
	}
}

void auto_run(void)
{
	if	(servoPOS < minAngle)		{		servoPOS = minAngle;		}
	if	(servoPOS > maxAngle)		{		servoPOS = maxAngle;		}
	
	if	(directionPOS)		{		servoPOS = servoPOS + 2;	}
	else									{		servoPOS = servoPOS - 2;	}
	
	if				(servoPOS >= maxAngle)		{		servoPOS = maxAngle;			directionPOS = false;		buzzer_beep(50);		ES_msDelay(50);	 	 buzzer_beep(50);		}
	else if		(servoPOS <= minAngle)		{		servoPOS = minAngle;			directionPOS = true;		buzzer_beep(50);		ES_msDelay(50);		 buzzer_beep(50);		}
	
	servo_setAngle(servoPOS);
	
	echoReady = false;
	ultrasonic_trigger();
	ES_msDelay(60);
	
	if(echoReady)	{		ES_printf(0, "\rAuto Angle: %3d Degrees  Distance: %3d cm		", servoPOS, ultrasonic_getCM());		}
	else					{		ES_printf(0, "\rAuto Angle: %3d Degrees  Distance: ------		", servoPOS);												}
	lcd_liveInfo(servoPOS);
}