#ifndef INCLUDES_H
#define INCLUDES_H

//library
#include "ES.h"
#include "LCD_Display.h"
#include "math.h"

//hardware
#include "BUZZER.h"
#include "POT.h"
#include "RADAR.h"
#include "LCD.h"
#include "SERVO.h"
#include "CONTROLLER.h"

//setup
#include "UART.h"
#include "ULTRASONIC.h"
#include "TICK.h"

//global define
#define white	ILI9341_WHITE
#define red		ILI9341_RED
#define green	ILI9341_GREEN
#define black	ILI9341_BLACK
#define blue	ILI9341_BLUE

#define IDLE				1
#define AUTOMATIC		2
#define MANUAL			3

#define SERVO_LOAD   39999
#define SERVO_MIN    2000
#define SERVO_MAX    4000


#endif