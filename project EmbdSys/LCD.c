#include "INCLUDES.h"

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
	
	setRotation(3);
//					X		 Y	  W		 H
	fillRect(159, 0, 	 3, 	240, 	green);		// middle line
	fillRect(1, 	45, 159, 	2, 		green);		// state line
	fillRect(1, 	158, 159, 2, 		green);		// error line
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
}


void lcd_error(char *reason, int x)
{
	setRotation(2);
	
	setCharConfig(red, 1, 2, black, 2);
	moveCursor(50, 77);			drawString("ERROR!", 6);
	setCharConfig(red, 1, 1, black, 1);
	moveCursor(47, 40);			drawString("REASON:", 7);
	setCharConfig(red, 1, 1, black, 1);
	moveCursor(x, 15);			drawString(reason, strlen(reason));
	
	for(int i = 0; i < 5; i++)		{		buzzer_beep(60);		ES_msDelay(60);		}
	ES_msDelay(1000);
	
	fillRect(0, 0, 80, 155, black);
	
	setRotation(3);
}


//------------------------------------------ OLD CODE --------------------------------------
/*
void lcd_radarBackground(void)
{
	fillScreen(ILI9341_BLACK);
	for(int r = RING_GAP; r <= RADAR_R; r = r + RING_GAP)
	{
		for(int a = 0; a <= 360; a++)
		{
			float rad = (a / 2.0) * PI / 180.0;
			drawPixel(RADAR_X + r * cos(rad), RADAR_Y - r * sin(rad), ILI9341_DARKGREEN);
		}
	}
	fillRect(RADAR_X - RADAR_R, RADAR_Y, RADAR_R * 2, 1, ILI9341_DARKGREEN);
}

static int lastAngle = 90;
static int lastCM = 0;

void lcd_radarLine(int angle)
{
	float oldRad = lastAngle * PI / 180;
	float newRad = angle * PI / 180;
	
	for(int dis = 0; dis <= RADAR_R; dis++)
	{
		int oldX = RADAR_X;
		int oldY = RADAR_Y;
		int newX = RADAR_X;
		int newY = RADAR_Y;
		// if distance between ring gap = 0 (%)??
		
	}
}

*/
