#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tice.h>
#include <graphx.h>
#include <debug.h>
#include <compression.h>

#include "misc.h"
#include "gfx/PKMNSD4.h"
#include "gfx/PKMNSD5.h"

/* For FindColors */
#include "map.h"
#include "menu.h"
#include "battle.h"

int ColorDifference(uint16_t color1, uint16_t color2);
void FindBestColors(void);

/* Multipurpose string variable */
char str[100];

uint16_t i;
uint16_t e;

bool debugging = 0;

#define COLORCOUNT 17


uint8_t colors[COLORCOUNT] = {0};


uint8_t colorSets[2][COLORCOUNT] = {
	{8,9,248,240,139,114,252,252,161,139,69,139,0,235,150,208,111,},
	{23,8,155,24,83,231,0,21,47,173,113,109,0,112,4,178,11,},
};

uint8_t colorValues[COLORCOUNT][3] = {
	{247,247,247},//0
	{0,0,0},//1
	{127,0,127},//2
	{255,0,0},//3
	{255,255,0},//4
	{128,128,128},//5
	{0,255,255},//6
	{64,200,248},//7
	{62,255,90},//8
	{248,224,56},//9
	{248,88,56},//10
	{248,224,0},//11
	{0,200,200},//12
	{0,128,255},//13
	{40,48,48},//14
	{138,138,200},//15
	{200,200,216},//16
};

gfx_sprite_t *typeIcons[17];
gfx_sprite_t *statusIcons[5];
gfx_sprite_t *categoryIcons[3];

/* Wait 10ms times amount */
void Wait(uint16_t amount) {
	while (amount--)
	{
		boot_WaitShort();
	}
}

/* Changes the colors list to one for the current palette and sets transparent text color */
void SetColors(uint8_t gameState) {
	int colorIndex;
	for (colorIndex = 0; colorIndex < sizeof(colors); colorIndex++) {
		colors[colorIndex] = colorSets[gameState][colorIndex];
		
	}
	gfx_SetTextTransparentColor(colors[12]);
	gfx_SetTextBGColor(colors[12]);
}

/* Converts the RGB data from colorValues to 1555 data for colorSets and prints it in the debug console */
void FindColors(void) {
#ifndef NDEBUG
	gfx_SetPalette(map_gfx_pal, sizeof_map_gfx_pal, 0);
	FindBestColors();
	gfx_SetPalette(battle_gfx_pal, sizeof_battle_gfx_pal, 0);
	FindBestColors();
#else
	
#endif
}
void FindBestColors(void) {
	int paletteIndex, colorIndex, lowestDifference, bestColor;
	dbg_sprintf(dbgout, "\n\t{");
	for (colorIndex = 0; colorIndex < COLORCOUNT; colorIndex++) {
		lowestDifference = 10000;
		bestColor = 0;
		for (paletteIndex = 0; paletteIndex < 256; paletteIndex++) {
			if (ColorDifference(gfx_RGBTo1555(colorValues[colorIndex][0], colorValues[colorIndex][1], colorValues[colorIndex][2]), gfx_palette[paletteIndex]) < lowestDifference) {
				lowestDifference = ColorDifference(gfx_RGBTo1555(colorValues[colorIndex][0], colorValues[colorIndex][1], colorValues[colorIndex][2]), gfx_palette[paletteIndex]);
				bestColor = paletteIndex;
				//dbg_sprintf(dbgout, "\nNew Best Color: %d / %u / %u,%u,%u", bestColor, gfx_palette[bestColor], color_1555ToRGB(gfx_palette[bestColor])[0], color_1555ToRGB(gfx_palette[bestColor])[1], color_1555ToRGB(gfx_palette[bestColor])[2]);
				//dbg_sprintf(dbgout, "\nDifference: %u", ColorDifference(gfx_RGBTo1555(colorValues[colorIndex][0], colorValues[colorIndex][1], colorValues[colorIndex][2]), gfx_palette[paletteIndex]));
			}
		}
		//dbg_sprintf(dbgout, "\nFinal Best Color: %d / %u / %u,%u,%u\n\n\n", bestColor, gfx_palette[bestColor], color_1555ToRGB(gfx_palette[bestColor])[0], color_1555ToRGB(gfx_palette[bestColor])[1], color_1555ToRGB(gfx_palette[bestColor])[2]);
		dbg_sprintf(dbgout, "%d,", bestColor);
	}
	dbg_sprintf(dbgout, "},");
}

int ColorDifference(uint16_t color1, uint16_t color2) {
	return(abs(((int)(color_1555ToRGB(color1)[0])) - ((int)(color_1555ToRGB(color2)[0]))) + abs(((int)(color_1555ToRGB(color1)[1])) - ((int)(color_1555ToRGB(color2)[1]))) + abs(((int)(color_1555ToRGB(color1)[2])) - ((int)(color_1555ToRGB(color2)[2]))));
}


uint8_t * color_1555ToRGB(uint16_t input) {
	uint8_t output[3];

	output[0] = (uint8_t)(((input & 0xFC00) >> 10) << 3);
	output[1] = (uint8_t)(((input & 0x03E0) >> 5) << 3);
	output[2] = (uint8_t)((input & 0x001F) << 3);

	return output;
}


void MallocIcons(void) {
	int iconIndex;
	for (iconIndex = 0; iconIndex < 17; iconIndex++) {
		typeIcons[iconIndex] = gfx_MallocSprite(32, 12);
		zx7_Decompress(typeIcons[iconIndex], PKMNSD5[iconIndex]);
	}
	for (iconIndex = 0; iconIndex < 5; iconIndex++) {
		statusIcons[iconIndex] = gfx_MallocSprite(20, 8);
		zx7_Decompress(statusIcons[iconIndex], PKMNSD5[17 + iconIndex]);
	}
	for (iconIndex = 0; iconIndex < 3; iconIndex++) {
		categoryIcons[iconIndex] = gfx_MallocSprite(32, 14);
		zx7_Decompress(categoryIcons[iconIndex], PKMNSD5[22 + iconIndex]);
	}
}
void FreeIcons(void) {
	int iconIndex;
	for (iconIndex = 0; iconIndex < 17; iconIndex++) {
		free(typeIcons[iconIndex]);
	}
	for (iconIndex = 0; iconIndex < 5; iconIndex++) {
		free(statusIcons[iconIndex]);
	}
	for (iconIndex = 0; iconIndex < 3; iconIndex++) {
		free(categoryIcons[iconIndex]);
	}
}