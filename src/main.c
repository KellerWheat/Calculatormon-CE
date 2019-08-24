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

	map_Initialize();
	battle_Initialize();


	FindColors();

	map_Setup();

	map_LoadPokeballs();



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
	} while (gameState < 2);

	map_End();


	gfx_End();
}