#ifndef SERVO_H
#define SERVO_H

#include "INCLUDES.h"

void GPIOF_setup(void);
void initSERVO(void);
void servo_setAngle(int angle);

#endif