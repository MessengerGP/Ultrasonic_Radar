#include "SERVO.h"

void GPIOF_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 5);                 // Port F clock
	while((SYSCTL -> PRGPIO & (1 << 5)) == 0);      // wait until ready
	
	GPIOF_AHB -> AFSEL |= (1 << 2);                 // PF2 alternate function
	GPIOF_AHB -> PCTL &= ~0xF00;     								// clear PF2 field
	GPIOF_AHB -> PCTL |= 0x600;      								// PF2 = M0PWM2 (col 6)
	GPIOF_AHB -> DEN |= (1 << 2);                   // digital enable PF2
}

void initSERVO(void)
{
	GPIOF_setup();
	
	SYSCTL -> RCGCPWM |= (1 << 0);                  // PWM module 0 clock (safe if buzzer already did it)
	while((SYSCTL -> PRPWM & (1 << 0)) == 0);       // wait until ready
	
	PWM0 -> CC = 0x102;                             // use divider, /8 -> 2 MHz (same as buzzer)
	
	PWM0 -> _1_CTL = 0x00;                          // disable generator 1 while configuring
	PWM0 -> _1_GENA = 0x8C;                         // high on LOAD, low on CMPA (down-count)
	PWM0 -> _1_LOAD = SERVO_LOAD;                   // 20 ms period (50 Hz)
	PWM0 -> _1_CMPA = SERVO_LOAD - ((SERVO_MIN + SERVO_MAX) / 2);   // start at middle (90 deg)
	PWM0 -> _1_CTL = 0x01;                         // enable generator 1
	
	PWM0 -> ENABLE |= (1 << 2);                     // M0PWM2 output on
}

void servo_setAngle(int angle)
{
	uint32_t pulse;
	
	if(angle < 0)   angle = 0;                      // clamp to 0 - 180
	if(angle > 180) angle = 180;
	
	pulse = SERVO_MIN + ((uint32_t)angle * (SERVO_MAX - SERVO_MIN)) / 180;
	
	PWM0 -> _1_CMPA = SERVO_LOAD - pulse;           // high time = LOAD - CMPA
}