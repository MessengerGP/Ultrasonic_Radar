#ifndef SERVO_H
#define SERVO_H

#include "ES.h"

#define SERVO_LOAD   39999      // 2 MHz / 50 Hz - 1 -> 20 ms period
#define SERVO_MIN    2000       // 1.0 ms pulse (0 deg)   - tune later
#define SERVO_MAX    4000       // 2.0 ms pulse (180 deg) - tune later

void GPIOF_setup(void);
void initSERVO(void);
void servo_setAngle(int angle);

#endif