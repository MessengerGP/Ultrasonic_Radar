#include "RADAR.h"

#include "POT.h"
#include "SERVO.h"
#include "ULTRASONIC.h"


int state = IDLE;
int minAngle = 0;
int maxAngle = 180;


void change_state(char c)
{
	if(c == 27)//esc = ASCII value of 27
	{
		idle_state();
	}
	else if(c == 'a' || c == 'A')
	{
		auto_state();
	}
	else if(c == 'm' || c == 'M')
	{
		manual_state();
	}
}


void set_bounds(char key)
{
	if(state == IDLE)
	{
		if(key == 60)//ASCII for '<' = 60
		{
			minAngle++;
			if(minAngle > 180)
			{
				minAngle = 180;
			}
			ES_printf(0, "MIN ANGLE RANGE: %d\nMAX ANGLE RANGE: %d\n\n--------------------------\n\n", minAngle, maxAngle);
		}
		else if(key == 62)//ASCII for '>' = 62
		{
			maxAngle--;
			if(maxAngle < 0)
			{
				maxAngle = 0;
			}
			ES_printf(0, "MIN ANGLE RANGE: %d\nMAX ANGLE RANGE: %d\n\n--------------------------\n\n", minAngle, maxAngle);
		}
	}
}

void idle_state(void)
{
	state = IDLE;
	//placeholder for code since this requires buzzer that doesnt exist yet
	//angle min and max wont do anything
}

void auto_state(void)
{
	state = AUTOMATIC;
	//wihtout input, angle will loop through min to max indefinitly
}

void manual_state(void)
{
	state = MANUAL;
	//angle will require inputs for lowering to min and raising to max ( < and > )
}

void manual_run(void)
{
	int angle;
	
	ADC_start();
	while(!adcReady);
	adcReady = false;
	
	angle = potValue * 180 / 4095;
	servo_setAngle(angle);
	
	echoReady = false;
	ultrasonic_trigger();
	ES_msDelay(60);
	
	if(echoReady){
		ES_printf(0, "\rAngle: %3d Degrees		Distance: %3d cm		", angle, ultrasonic_getCM());
	}
	else{
		ES_printf(0, "\rAngle: %3d Degrees		Distance: ------		", angle);
	}
	
}

