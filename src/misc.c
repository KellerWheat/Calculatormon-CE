#include <stdint.h>
#include <tice.h>
#include <graphx.h>

#include "misc.h"

char str[100];

uint16_t i;
uint16_t e;

/* white, black, poison, burn, paralysis, sleep, frozen*/
uint8_t colors[7] = { 255,254,253,252,251,250,249};


uint8_t colorSets[3][7] = {
	{255,18,0,0,0,0,0},
	{0x00,0xFA,0xCE,0xD4,0x28,0xD9,0xF9},
	{0x00,0x01,0x05,0x04,0x02,0x03,0x06},
};

/* Wait 10ms times amount */
void Wait(uint16_t amount) {
	while (amount--)
	{
		boot_WaitShort();
	}
}

/* Changes the colors list to one for the current palette */
void SetColors(uint8_t gameState) {
	int colorIndex;
	for (colorIndex = 0; colorIndex < sizeof(colors); colorIndex++) {
		colors[colorIndex] = colorSets[gameState][colorIndex];
		
	}
}