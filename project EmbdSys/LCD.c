#include "INCLUDES.h"

static int echoDotX = -1;
static int echoDotY = -1;


// ----------------------------------------- SETUP -----------------------------------------
void GPIOD_setup(void)
{
	SYSCTL -> RCGCGPIO |= (1 << 3);
	while((SYSCTL -> PRGPIO & (1 << 3)) == 0);
	
	GPIOD_AHB -> DIR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> DEN |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> PUR |= (1 << 3);
	GPIOD_AHB -> AFSEL |= (1 << 1) | (1 << 2) | (1 << 3);
	GPIOD_AHB -> PCTL &= ~((0xF << 4) | (0xF << 8) | (0xF << 12));
	GPIOD_AHB -> PCTL |= (0xF << 4) | (0xF << 8) | (0xF << 12);
}

void initSPI(void)
{
	GPIOD_setup();
	
	SYSCTL -> RCGCSSI |= (1 << 2);
	while((SYSCTL -> PRSSI & (1 << 2)) == 0);
	
	SSI2 -> CR1 &= ~((1 << 1) | (1 << 2)); 
	SSI2 -> CPSR = 8; 																//changed from 4 to 8 (2MHz)
	SSI2 -> CR0 = (1 << 7) | (1 << 6) | (7 << 0);										
	SSI2 -> CR1 |= (1 << 1); 
}

void spi_Transmit(uint8_t data)
{
	while(!(SSI2 -> SR & (1 << 1)));
	SSI2 -> DR = data;
	while(SSI2 -> SR & (1 << 4));
}

// ----------------------------------------- DISPLAY -----------------------------------------

// ------------------------------------ LEFT SIDE OF LCD -------------------------------------
void lcd_modeInfo(void)
{
	char text[20];
	
	static int lastState = 0;
	if(state != lastState)	{	 fillRect(0, 0, 159, 240, black);		lastState = state;	}
	
	setRotation(2);
	
	setCharConfig(white, 2, 2, black, 2);
	if				(state == IDLE)				{		moveCursor(48, 230);		drawString("IDLE  ", 6);		}
	else if		(state == AUTOMATIC)	{		moveCursor(32, 230);		drawString("AUTO  ", 6);		}
	else if		(state == MANUAL)			{		moveCursor(11, 230);		drawString("MANUAL", 6);		}
	
	setCharConfig(white, 1, 1, black, 1);
	if(state == IDLE)
	{
		moveCursor(11, 180);			drawString("MIN", 3);
		moveCursor(11, 155);			drawString("MAX", 3);
		moveCursor(55, 180);			drawString("ANGLE:", 6);
		moveCursor(55, 155);			drawString("ANGLE:", 6);
		
		sprintf(text, "%03d", minAngle);
		moveCursor(120, 180);
		drawString(text, strlen(text));
		
		sprintf(text, "%03d", maxAngle);
		moveCursor(120, 155);
		drawString(text, strlen(text));
	}
	else if(state == AUTOMATIC)
	{
		moveCursor(11, 155);			drawString("DIST:", 5);
		moveCursor(11, 180);			drawString("ANGLE:", 6);
		moveCursor(115, 155);			drawString("cm", 2);
	}
	else if(state == MANUAL)
	{
		moveCursor(11, 180);			drawString("MODE:", 5);
		moveCursor(11, 130);			drawString("DIST:", 5);
		moveCursor(11, 155);			drawString("ANGLE:", 6);
		moveCursor(115, 130);			drawString("cm", 2);
		moveCursor(115, 155);			drawString("deg", 3);
		
		if(joystickCTRL)
		{
			moveCursor(70, 180);			drawString("JOYSTICK", 8);
		}
		else
		{
			setCharConfig(black, 1, 1, black, 1);
			moveCursor(70, 180);			drawString("JOYSTICK", 8);
			
			setCharConfig(white, 1, 1, black, 1);
			moveCursor(70, 180);			drawString("POT", 3);
		}
	}

	setCharConfig(white, 1, 1, black, 1);
	moveCursor(11, 100);			drawString("ECHO:", 5);
	moveCursor(115, 100);			drawString("cm", 2);
	moveCursor(75, 100);			drawString(echoText, strlen(echoText));
	
	setRotation(3);
	
	fillRect	(159, 0, 	 3, 	240, 	green);		// middle line
	fillRect	(1, 	45, 159, 	2, 		green);		// state line
	fillRect	(1, 	158, 159, 2, 		green);		// error line
	lcd_gauge	(radar_getAngle());
}


void lcd_liveInfo(int angle)
{
	char angleText[10];
	char distText[10] = "000";
	
	sprintf(angleText, "%03d", angle);

	int cm = ultrasonic_getCM();
	if(echoReady && cm <= 200)	{		sprintf(distText, "%03d", cm);		}
	
	setRotation(2);
	setCharConfig(white, 1, 1, black, 1);
	
	if(state == AUTOMATIC)
	{
		moveCursor(75, 180);		drawString(angleText, strlen(angleText));
		moveCursor(75, 155);		drawString(distText, strlen(distText));
	}
	else if(state == MANUAL)
	{
		moveCursor(75, 155);		drawString(angleText, strlen(angleText));
		moveCursor(75, 130);		drawString(distText, strlen(distText));
	}
	
	setRotation(3);
	
	lcd_sonarLine		(angle);
	lcd_sonarMap		(angle);
	lcd_gauge				(angle);
}


void lcd_error(char *reason, int x)
{
	setRotation(2);
	
	setCharConfig	(red, 1, 2, black, 2);
	moveCursor		(50, 77);		drawString("ERROR!", 6);
	setCharConfig(red, 1, 1, black, 1);
	moveCursor		(47, 40);		drawString("REASON:", 7);
	setCharConfig	(red, 1, 1, black, 1);
	moveCursor		(x, 15);		drawString(reason, strlen(reason));
	
	for(int i = 0; i < 5; i++)		{		buzzer_beep(60);		ES_msDelay(60);		}
	ES_msDelay(1000);
	
	fillRect(0, 0, 80, 155, black);
	
	setRotation(3);
}

// ------------------------------------ RIGHT SIDE OF LCD -------------------------------------

void lcd_sonarBase(void)
{
	setRotation(3);

	int X 			= 240;
	int Y 			= 88;
	int radiusX = 75;
	int radiusY = 45;

	
	for(int angle = 0; angle <= 180; angle++)
	{
		float rad = angle * PI / 180;
		fillRect(X + radiusX * cos(rad), Y + radiusY * sin(rad), 1, 1, darkGreen);
	}
	
	int 	rightLineX 	= 260;
	int 	rightLineY 	= 17;
	float rightLine 	= 65 * PI / 180;
	
	for(int length = 0; length <= 175; length++)
	{
		float size = length / 100.0;
		fillRect(rightLineX + size * radiusX * cos(rightLine), rightLineY + size * radiusY * sin(rightLine), 1, 1, darkGreen);
	}
	
	int 	leftLineX 	= 220;
	int 	leftLineY 	= 17;
	float leftLine 		= 115 * PI / 180;
	
	for(int length = 0; length <= 175; length++)
	{
		float size = length / 100.0;
		fillRect(leftLineX + size * radiusX * cos(leftLine), leftLineY + size * radiusY * sin(leftLine), 1, 1, darkGreen);
	}
	
	fillRect(222,5,37,5,  blue);    // base
	fillRect(228,7,8,7,   blue);	  // left eye
	fillRect(245,7,8,7,   blue);	  // right eye
	fillRect(160,145,159,2, green); // right boarder
	
	fillRect   (170, 187, 141, 2, white);		// top
	fillRect   (170, 223, 141, 2, white);		// bottom
	fillRect   (170, 187, 2, 37, white);		// left
	fillRect   (309, 187, 2, 37, white);		// right
	
	setRotation(2);
	setCharConfig(white, 1, 2, black, 2);
	moveCursor(170, 88);		drawString("RANGE:", 6);
	setRotation(3);
}

void lcd_sonarLine(int servoAngle)
{
	static int lastEndX = 240;
	static int lastEndY = 133;
	
	int startX 	= 240;
	int startY 	= 17;
	int X 			= 240;
	int Y 			= 88;
	int radiusX = 75;
	int radiusY = 45;
	
	float arcRad 	= (180 - servoAngle) * PI / 180;
	int endX 			= X + radiusX * cos(arcRad);
	int endY 			= Y + radiusY * sin(arcRad);
	
	if(endX == lastEndX && endY == lastEndY)	{	return;	}
	
	for(int length = 0; length <= 100; length++)
	{
		float size = length / 100.0;
		fillRect(startX + size * (lastEndX - startX), startY + size * (lastEndY - startY), 1, 1, black);
	}

	for(int length = 0; length <= 95; length++)
	{
		float size = length / 100.0;
		fillRect(startX + size * (endX - startX), startY + size * (endY - startY), 1, 1, green);
	}
	
	lastEndX = endX;
	lastEndY = endY;
}


void lcd_sonarMap(int servoAngle)
{
	static int dotX[37];
	static int dotY[37];
	static bool hasDot[37];
	static uint32_t lastRecord = 0;
	
	
	int startX 	= 240;
	int startY 	= 17;
	int X 			= 240;
	int Y 			= 88;
	int radiusX = 75;
	int radiusY = 45;
	
	int slot 	= servoAngle / 5;
	int cm 		= ultrasonic_getCM();
	
	if((msTick - lastRecord) >= 1500)
	{
		if(hasDot[slot])	{		fillRect(dotX[slot] - 2, dotY[slot] - 2, 4, 4, black);		hasDot[slot] = false;		}
		
		if(echoReady && cm > 20 && cm <= 200)
		{
			float arcRad 		= (180 - servoAngle) * PI / 180;
			int 	endX 			= X + radiusX * cos(arcRad);
			int 	endY		 	= Y + radiusY * sin(arcRad);
			float size 			= (cm / 200.0) * 0.90;
			
			dotX	[slot] 		= startX + size * (endX - startX);
			dotY	[slot] 		= startY + size * (endY - startY);
			hasDot[slot] 		= true;
		}
		
		lastRecord = msTick;
	}
	
	for(int i = 0; i < 37; i++)
	{
		if(hasDot[i])			{		fillRect(dotX[i] - 2, dotY[i] - 2, 4, 4, red);			}
	}
	
	if(echoDotX != -1)	{		fillRect(echoDotX - 2, echoDotY - 2, 4, 4, blue);		}
}



void lcd_echoDot(int servoAngle, int cm)
{
	int startX 	= 240;
	int startY 	= 17;
	int X 			= 240;
	int Y 			= 88;
	int radiusX = 75;
	int radiusY = 45;
	
	if(echoDotX != -1)	{	fillRect(echoDotX - 2, echoDotY - 2, 4, 4, black);		echoDotX = -1;	}
	
	if(cm != -1)
	{
		float arcRad 	= (180 - servoAngle) * PI / 180;
		int endX 			= X + radiusX * cos(arcRad);
		int endY 			= Y + radiusY * sin(arcRad);
		float size 		= (cm / 200.0) * 0.90;
		
		echoDotX = startX + size * (endX - startX);
		echoDotY = startY + size * (endY - startY);
		fillRect(echoDotX - 2, echoDotY - 2, 4, 4, blue);
	}
}



void lcd_gauge(int angle)
{
	static int lastAngleX = -1;
	static int lastMinX = -1;
	static int lastMaxX = -1;
	
	int angleX 	= 180 + angle 		* 120 / 180;
	int minX 		= 180 + minAngle 	* 120 / 180;
	int maxX 		= 180 + maxAngle 	* 120 / 180;
	
	if(lastAngleX != -1)		{		fillRect(lastAngleX, 195, 2, 21, black);		}
	if(lastMinX != -1)			{		fillRect(lastMinX, 199, 3, 13, black);			}
	if(lastMaxX != -1)			{		fillRect(lastMaxX, 199, 3, 13, black);			}
	
	fillRect(180, 205, 121, 1, white);
	fillRect(180, 201, 1, 9, white);
	fillRect(300, 201, 1, 9, white);
	
	if(state == AUTOMATIC)
	{
		fillRect(minX, 199, 3, 13, red);
		fillRect(maxX, 199, 3, 13, red);
		lastMinX = minX;
		lastMaxX = maxX;
	}
	else
	{
		lastMinX = -1;
		lastMaxX = -1;
	}
	
	fillRect(angleX, 195, 2, 21, green);
	lastAngleX = angleX;
}
