/*
 *--------------------------------------
 * Program Name: Pokemon
 * Author: FBDAJH
 * License: MIT License
 * Description: Pokemon
 *--------------------------------------
*/

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* More headers */
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

/* Include Data */
#include "text.h"
#include "misc.h"
#include "data.h"
#include "savegame.h"
#include "stats.h"
#include "map.h"
#include "battle.h"
#include "menu.h"
#include "items.h"

#include "gfx/battle_gfx.h"
#include "gfx/map_gfx.h"

#include "gfx/PKMNSD0.h"
#include "gfx/PKMNSD1.h"
#include "gfx/PKMNSD2.h"
#include "gfx/PKMNSD3.h"
#include "gfx/PKMNSD4.h"
//#include "gfx/PKMNSD5.h"
#include "gfx/PKMNSD6.h"
#include "gfx/PKMNSD7.h"
#include "gfx/PKMNSD8.h"


/* Variables */

uint8_t gameState = 0;

void main(void) {
	/* Seed RNG */
	unsigned seed = rtc_Time();

	/* Setup gfx */
	dbg_ClearConsole();
	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetMonospaceFont(8);

	/* Game */

	srand(seed);

	PKMNSD0_init();
	PKMNSD1_init();
	PKMNSD2_init();
	PKMNSD3_init();
	PKMNSD4_init();
	//PKMNSD5_init();
	PKMNSD6_init();
	PKMNSD7_init();
	PKMNSD8_init();

	map_Initialize();
	battle_Initialize();

	if (kb_Data[3] & kb_0) {
		debugging = true;
	}


	FindColors();

	map_Setup();

	map_LoadPokeballs();

	kb_Scan();
	
	do {
		kb_Scan();
		if (gameState == 0) {
			gameState = map_Loop();
			
			if (gameState == 1) {
				map_End();
				battle_Setup();
			}
			if (gameState == 2) {
				map_End();
			}
		}
		else if (gameState == 1) {
			gameState = battle_Loop();
			if (gameState == 0) {
				battle_End();
				map_Setup();
			}
			if (gameState == 2) {
				battle_End();
			}
		}
		if (debugging) {
			if (kb_Data[4] & kb_DecPnt) {
				Wait(15);
			}
		}
	} while (gameState < 2);


	gfx_End();
}