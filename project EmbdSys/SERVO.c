#include "SERVO.h"

void GPIOF_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 5);                
	while((SYSCTL -> PRGPIO & (1 << 5)) == 0);    
	
	GPIOF_AHB -> AFSEL |= (1 << 2);                 
	GPIOF_AHB -> PCTL &= ~(0xF << 8);
	GPIOF_AHB -> PCTL |= (6 << 8);		
	GPIOF_AHB -> DEN |= (1 << 2);          
}

void initSERVO(void)
{
	GPIOF_setup();
	
	SYSCTL -> RCGCPWM |= (1 << 0);                 
	while((SYSCTL -> PRPWM & (1 << 0)) == 0); 
	
	PWM0 -> CC = (1 << 8) | (2 << 0);	
	
	PWM0 -> _1_CTL = 0;                          								// turn off generator 1 while setting it up
	PWM0 -> _1_GENA = (1 << 2) | (1 << 3) | (1 << 7);         			// pin goes high at start of cycle, low at CMPA
	PWM0 -> _1_LOAD = SERVO_LOAD;                   								// 20 ms cycle (50 Hz) for the servo
	PWM0 -> _1_CMPA = SERVO_LOAD - ((SERVO_MIN + SERVO_MAX) / 2);   // start in the middle (90 degrees)
	PWM0 -> _1_CTL = (1 << 0);                          								// turn generator 1 back on
	
	PWM0 -> ENABLE |= (1 << 2);                     								// turn on PF2 output to the servo
}

void servo_setAngle(int angle)
{
	uint32_t pulse;
	
	if	(angle < 0)		{		angle = 0;		}		// keep angle between 0 and 180
	if	(angle > 180)	{		angle = 180;	}
	
	pulse = SERVO_MIN + ((uint32_t)angle * (SERVO_MAX - SERVO_MIN)) / 180;		// turn angle into a pulse width
	
	PWM0 -> _1_CMPA = SERVO_LOAD - pulse;           // set the pulse width (high time = LOAD - CMPA)
}
