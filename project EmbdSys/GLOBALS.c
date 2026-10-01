#include "INCLUDES.h"

volatile char holdingLastPressed;
volatile bool hasButtonPressed = false;

volatile uint32_t msTick = 0;

volatile uint16_t potValue = 0;
volatile uint16_t joyX = 0;
volatile uint16_t joyY = 0;
volatile bool adcReady = false;

volatile bool echoReady = false;
char echoText[10] = "N/A";

int state = IDLE;
int minAngle = 0;
int maxAngle = 180;
bool joystickCTRL = false;
