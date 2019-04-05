/*
 *--------------------------------------
 * Program Name: Pokemon
 * Author: FBDAJH
 * License: MIT License
 * Description: Pokemon (pokemon and mechanics are mostly from gen 1)
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

/* Variables */

uint8_t gameState = 0;

void main(void) {
	int colorIndex;
	/* Seed RNG */
	unsigned seed = rtc_Time();

	/* Setup gfx */
	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetMonospaceFont(8);

	/* Game */

	srand(seed);
	
	save_Load();

	map_Initialize();
	battle_Initialize();
	menu_Initialize();

	map_Setup();


	/* Debug all colors */
	dbg_sprintf(dbgout, "\nColors:\n");
	for (colorIndex = 0; colorIndex < 256; colorIndex++) {
		dbg_sprintf(dbgout, "%u : %u\n", colorIndex, gfx_palette[colorIndex]);
	}


	if (newGame) {
		uint8_t starter = 0;
		newGame = false;
		text_Display("Choose Starter", false);
		while (starter == 0) {
			starter = text_AskQuestion4("Bulbasaur", "Charmander", "Squirtle", " ", false);
		}
		if (starter == 4) {
			party[0] = stats_NewCharacter(151, 100);
		}
		else {
			party[0] = stats_NewCharacter(1 + ((starter - 1) * 3), 5);
		}
	}

	map_Setup();

	do {
		kb_Scan();
		if (gameState == 0) {
			gameState = map_Loop();
			if (gameState == 1) {
				battle_Setup();
			}
			if (gameState == 2) {
				menu_Setup();
			}
		}
		else if (gameState == 1) {
			gameState = battle_Loop();
			if (gameState == 0) {
				map_Setup();
			}
			if (gameState == 2) {
				menu_Setup();
			}
		}
		else if (gameState == 2) {
			gameState = menu_Loop();
			if (gameState == 0) {
				map_Setup();
			}
			if (gameState == 1) {
				battle_Setup();
			}
		}
	} while (gameState < 3);
	gfx_End();
}