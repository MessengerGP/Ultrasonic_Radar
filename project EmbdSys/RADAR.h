#ifndef RADAR_H
#define RADAR_H

#include "ES.h"

#define IDLE				1
#define AUTOMATIC		2
#define MANUAL			3


extern int state;
extern int minAngle;
extern int maxAngle;
extern volatile int angle;


void change_state(char c);

void set_bounds(char key);

void idle_state(void);

void auto_state(void);

void manual_state(void);

#endif


