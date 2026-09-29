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
