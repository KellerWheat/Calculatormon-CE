#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <keypadc.h>
#include <graphx.h>

#include "map.h"
#include "battle.h"
#include "menu.h"
#include "misc.h"
#include "data.h"
#include "text.h"
#include "savegame.h"
#include "stats.h"
#include "gfx/menu_gfx.h"
#include "items.h"

void MoveMenuCursor(int max);

int MainMenu(void);
void PokemonMenu(void);
void ItemMenu(void);
void FastTravelMenu(void);

/* Press enter to toggle; allows player to switch pokemon around */
bool switchMode;
uint8_t pokemonToSwitch;
struct pokemonData tempcharacter;

uint8_t usableItems[185];
uint8_t usableItemCount;

const uint8_t menuStatusColors[5] = { 3,4,5,6,7 };

uint8_t menuState1 = 0;
uint8_t menuState2 = 0;
uint8_t menuCurrent = 1;

void menu_Initialize(void) {
	
}
void menu_Setup(void) {
	int itemIndex;
	gfx_SetPalette(menu_gfx_pal, sizeof_menu_gfx_pal, 0);
	SetColors(2);
	gfx_SetTextFGColor(colors[1]);
	menuState1 = 0;
	menuState2 = 0;
	menuCurrent = 1;

	/* Generate Usable Items List */
	usableItemCount = 0;
	for (itemIndex = 0; itemIndex < 185; itemIndex++) {
		if (playerItems[itemIndex] > 0) {
			usableItems[usableItemCount] = itemIndex;
			usableItemCount++;
		}
	}
}
int menu_Loop(void) {
	if (menuState1 == 0) {
		return MainMenu();
	}
	else if (menuState1 == 1) {
		PokemonMenu();
	}
	else if (menuState1 == 2) {
		ItemMenu();
	}
	else if (menuState1 == 3) {
		FastTravelMenu();
	}
	return 2;
}

int MainMenu(void) {
	gfx_SetTextScale(2, 2);
	if (kb_Data[6] & kb_Clear) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return 0;
	}

	MoveMenuCursor(6);
	if (kb_Data[1] & kb_2nd) {
		gfx_SetTextScale(1, 1);
		menuState1 = menuCurrent;
		menuState2 = 0;
		menuCurrent = 1;
		switchMode = false;


		/* Items */
		if (menuState1 == 2) {
			menuCurrent = 0;
		}
		/* Check if fast travel availible */
		if (menuState1 == 3 && badgeCount == 0) {
			text_Display("Beat Gyms to unlock", false);
			menuCurrent = 0;
		}
		/* Save */
		if (menuState1 == 4) {
			save_Save();
			return 0;
		}
		/* Exit menu */
		if (menuState1 == 5) {
			return 0;
		}
		/* Exit game */
		if (menuState1 == 6) {
			return 3;
		}
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return 2;
	}

	gfx_FillScreen(colors[0]);
	gfx_TransparentSprite(menucursor, 20, menuCurrent * 30);
	gfx_PrintStringXY("Pokemon", 40, 30);
	gfx_PrintStringXY("Items", 40, 60);
	gfx_PrintStringXY("Fast Travel", 40, 90);
	gfx_PrintStringXY("Save", 40, 120);
	gfx_PrintStringXY("Close Menu", 40, 150);
	gfx_PrintStringXY("Exit Game", 40, 180);

	gfx_SwapDraw();
	return 2;
}
void PokemonMenu(void) {
	if (menuState2 == 0) {
		if (kb_Data[6] & kb_Clear) {
			menuState1 = 0;
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
			return;
		}

		MoveMenuCursor(6);
		if (kb_Data[6] & kb_Enter) {
			switchMode = !switchMode;
			pokemonToSwitch = 100;
			while (kb_Data[6] & kb_Enter) { kb_Scan(); }
		}
		if (kb_Data[1] & kb_2nd) {
			if (switchMode) {
				if (pokemonToSwitch == 100) {
					pokemonToSwitch = menuCurrent - 1;
				}
				else {
					tempcharacter = party[pokemonToSwitch];
					party[pokemonToSwitch] = party[menuCurrent - 1];
					party[menuCurrent - 1] = tempcharacter;
					pokemonToSwitch = 100;
				}
			}
			else {
				menuState2 = menuCurrent;
			}
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
			return;
		}


		gfx_FillScreen(colors[0]);
		if (switchMode) {
			gfx_TransparentSprite(movecursor, 20, menuCurrent * 30);
		}
		else {
			gfx_TransparentSprite(menucursor, 20, menuCurrent * 30);
		}

		i = 0;
		while (i < 6)
		{
			if (party[i].id != 0) {
				gfx_PrintStringXY(data_pokemon[party[i].id].name, 40, (30 * i) + 30);

				sprintf(str, "LV%u", party[i].level);
				gfx_PrintStringXY(str, 40, (30 * i) + 40);

				sprintf(str, "%u/%u ", party[i].currenthealth, stats_CalculateStats(party[i]).health);
				gfx_PrintStringXY(str, 100, (30 * i) + 40);
			}
			i++;
		}
	}
	/* Show data on selected character */
	else if (menuState2 > 0) {
		if (kb_Data[6] & kb_Clear) {
			menuState2 = 0;
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
			return;
		}
		gfx_FillScreen(colors[0]);
		gfx_SetColor(colors[1]);
		gfx_SetTextScale(2, 2);
		gfx_PrintStringXY(data_pokemon[party[menuState2 - 1].id].name, 20, 20);
		gfx_SetTextScale(1, 1);

		sprintf(str, "%s %s ", elementNames[data_pokemon[party[menuState2 - 1].id].element1], elementNames[data_pokemon[party[menuState2 - 1].id].element2]);
		gfx_PrintStringXY(str, 200, 26);

		gfx_PrintStringXY("Hp", 20, 50);
		sprintf(str, "%u/%u ", party[menuState2 - 1].currenthealth, stats_CalculateStats(party[menuState2 - 1]).health);
		gfx_PrintStringXY(str, 70, 50);

		gfx_PrintStringXY("Atk", 20, 70);
		sprintf(str, "%u", stats_CalculateStats(party[menuState2 - 1]).attack);
		gfx_PrintStringXY(str, 70, 70);

		gfx_PrintStringXY("Def", 20, 90);
		sprintf(str, "%u", stats_CalculateStats(party[menuState2 - 1]).defence);
		gfx_PrintStringXY(str, 70, 90);

		gfx_PrintStringXY("SpAtk", 20, 110);
		sprintf(str, "%u", stats_CalculateStats(party[menuState2 - 1]).specialattack);
		gfx_PrintStringXY(str, 70, 110);

		gfx_PrintStringXY("SpDef", 20, 130);
		sprintf(str, "%u", stats_CalculateStats(party[menuState2 - 1]).specialdefence);
		gfx_PrintStringXY(str, 70, 130);

		gfx_PrintStringXY("Spd", 20, 150);
		sprintf(str, "%u", stats_CalculateStats(party[menuState2 - 1]).speed);
		gfx_PrintStringXY(str, 70, 150);

		sprintf(str, "LV%u", party[menuState2 - 1].level);
		gfx_PrintStringXY(str, 20, 180);

		sprintf(str, "%uXP", party[menuState2 - 1].xp);
		gfx_PrintStringXY(str, 20, 200);

		gfx_SetTextFGColor(colors[1]);
		if (party[menuState2 - 1].move1 != 0) {
			gfx_Rectangle(120, 40, 180, 45);
			gfx_PrintStringXY(data_moves[party[menuState2 - 1].move1].name, 130, 50);
			gfx_PrintStringXY(text_movecategories[data_moves[party[menuState2 - 1].move1].type], 130, 67);
			sprintf(str, "%u Power", data_moves[party[menuState2 - 1].move1].power);
			if (data_moves[party[menuState2 - 1].move1].type < 2) {
				gfx_PrintStringXY(str, 225, 67);
			}
			gfx_PrintStringXY(elementNames[data_moves[party[menuState2 - 1].move1].element], 225, 50);
		}
		if (party[menuState2 - 1].move2 != 0) {
			gfx_Rectangle(120, 90, 180, 45);
			gfx_PrintStringXY(data_moves[party[menuState2 - 1].move2].name, 130, 100);
			gfx_PrintStringXY(text_movecategories[data_moves[party[menuState2 - 1].move2].type], 130, 117);
			sprintf(str, "%u Power", data_moves[party[menuState2 - 1].move2].power);
			if (data_moves[party[menuState2 - 1].move2].type < 2) {
				gfx_PrintStringXY(str, 225, 117);
			}
			gfx_PrintStringXY(elementNames[data_moves[party[menuState2 - 1].move2].element], 225, 100);
		}
		if (party[menuState2 - 1].move3 != 0) {
			gfx_Rectangle(120, 140, 180, 45);
			gfx_PrintStringXY(data_moves[party[menuState2 - 1].move3].name, 130, 150);
			gfx_PrintStringXY(text_movecategories[data_moves[party[menuState2 - 1].move3].type], 130, 167);
			sprintf(str, "%u Power", data_moves[party[menuState2 - 1].move3].power);
			if (data_moves[party[menuState2 - 1].move3].type < 2) {
				gfx_PrintStringXY(str, 225, 167);
			}
			gfx_PrintStringXY(elementNames[data_moves[party[menuState2 - 1].move3].element], 225, 150);
		}
		if (party[menuState2 - 1].move4 != 0) {
			gfx_Rectangle(120, 190, 180, 45);
			gfx_PrintStringXY(data_moves[party[menuState2 - 1].move4].name, 130, 200);
			gfx_PrintStringXY(text_movecategories[data_moves[party[menuState2 - 1].move4].type], 130, 217);
			sprintf(str, "%u Power", data_moves[party[menuState2 - 1].move4].power);
			if (data_moves[party[menuState2 - 1].move4].type < 2) {
				gfx_PrintStringXY(str, 225, 217);
			}
			gfx_PrintStringXY(elementNames[data_moves[party[menuState2 - 1].move4].element], 225, 200);
		}
	}

	gfx_SwapDraw();
}
void ItemMenu(void) {
	if (kb_Data[6] & kb_Clear) {
		menuState1 = 0;
		menuCurrent = 1;
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return;
	}
	gfx_FillScreen(colors[0]);

	gfx_SetColor(colors[0]);
	gfx_FillRectangle(20, 20, 280, 200);
	gfx_SetColor(colors[1]);
	gfx_Rectangle(20, 20, 280, 200);
	gfx_SetColor(colors[0]);
	gfx_PrintStringXY(">", 25, 25 + menuCurrent * 20);

	/* Draw Items*/
	for (i = 0; i < 10 && (i + menuState2) < usableItemCount; i++) {
		items_IndexToName(str, usableItems[i + menuState2]);
		gfx_PrintStringXY(str, 35, 25 + i * 20);
		sprintf(str, "%u", playerItems[usableItems[i + menuState2]]);
		gfx_PrintStringXY(str, 170, 25 + i * 20);
	}
	gfx_PrintStringXY(">", 25, 25 + menuCurrent * 20);

	if (kb_Data[7] & kb_Up) {
		if (menuCurrent != 0) {
			menuCurrent--;
			Wait(20);
		}
	}
	if (kb_Data[7] & kb_Down) {
		if (menuCurrent < 9) {
			menuCurrent++;
			Wait(20);
		}
	}
	if (kb_Data[7] & kb_Right) {
		menuState2 += 10;
		Wait(20);
	}
	if (kb_Data[7] & kb_Left) {
		if (menuState2 != 0) {
			menuState2 -= 10;
		}
		Wait(20);
	}

	if (kb_Data[1] & kb_2nd) {
		if (menuCurrent + menuState2 < usableItemCount) {
			items_UseItem(usableItems[menuCurrent + menuState2]);
		}
		while (kb_Data[1] & kb_2nd) { kb_Scan(); }
	}
	gfx_SwapDraw();
}
void FastTravelMenu(void) {
	if (kb_Data[6] & kb_Clear) {
		menuState1 = 0;
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return;
	}
	gfx_FillScreen(colors[0]);
	gfx_SetTextScale(2, 2);
	gfx_PrintStringXY("Fast Travel", 20, 20);
	gfx_SetTextScale(1, 1);

	gfx_SwapDraw();
}

/* Changes menuCurrent */
void MoveMenuCursor(int max) {
	if (kb_Data[7] & kb_Up) {
		menuCurrent--;
		if (menuCurrent < 1) {
			menuCurrent = max;
		}
		Wait(20);
	}
	if (kb_Data[7] & kb_Down) {
		menuCurrent++;
		if (menuCurrent > max) {
			menuCurrent = 1;
		}
		Wait(20);
	}
}