#ifndef RADAR_H
#define RADAR_H

#include "INCLUDES.h"


extern int state;
extern int minAngle;
extern int maxAngle;
extern bool joystickCTRL;


void change_state(char c);
void set_bounds(char key);

void idle_state(void);
void auto_state(void);
void manual_state(void);

void manual_run(void);
void auto_run(void);

int radar_getAngle(void);

#endif


