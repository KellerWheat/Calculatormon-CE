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
#include <compression.h>
#include <debug.h>

#include "map.h"
#include "battle.h"
#include "menu.h"
#include "misc.h"
#include "data.h"
#include "text.h"
#include "savegame.h"
#include "stats.h"
#include "gfx/battle_gfx.h"
#include "items.h"
#include "gfx/PKMNSD0.h"
#include "gfx/PKMNSD1.h"
#include "gfx/PKMNSD2.h"
#include "gfx/PKMNSD3.h"
#include "gfx/PKMNSD5.h"

void MoveMenuCursor(int max);

void ItemMenu(void);
void FastTravelMenu(void);


/* Press enter to toggle; allows player to switch pokemon around */
bool switchMode;
uint8_t pokemonToSwitch;
struct pokemonData tempcharacter;



uint8_t menuState1 = 0;
uint8_t menuState2 = 0;
uint8_t menuCurrent = 1;

void menu_Setup(void) {

	dbg_SetWatchpoint(&party[0].id, 1);
	dbg_SetWatchpoint(&party[0].moves[0], 1);
	dbg_SetWatchpoint(&party[0].moves[1], 1);
	dbg_SetWatchpoint(&party[0].moves[2], 1);
	dbg_SetWatchpoint(&party[0].moves[3], 1);

	zx7_Decompress(textBoxSprite1, menutextbox1_compressed);
	zx7_Decompress(textBoxSprite2, menutextbox2_compressed);

	gfx_SetPalette(battle_gfx_pal, sizeof_battle_gfx_pal, 0);
	SetColors(1);
	gfx_SetTextFGColor(255);


}

uint8_t menu_Menu(void) {
	int8_t cursorState = 0;
	

	gfx_PrintStringXY("Pokemon", 215, 35);
	gfx_PrintStringXY("Bag", 215, 50);
	gfx_PrintStringXY("Close Menu", 215, 65);
	gfx_PrintStringXY("Save", 215, 80);
	gfx_PrintStringXY("Exit Game", 215, 95);
	gfx_PrintStringXY(">", 208, 35 + 15 * cursorState);

	gfx_SwapDraw();
	gfx_SetDrawScreen();

	while (!(kb_Data[6] & kb_Clear)) {
		kb_Scan();
		if ((kb_Data[7] & kb_Down)) {
			cursorState += 1;
			if (cursorState == 5) {
				cursorState = 0;
			}
			gfx_FillRectangle(205, 35, 10, 70);
			gfx_PrintStringXY(">", 208, 35 + 15 * cursorState);
			Wait(20);
		}
		if ((kb_Data[7] & kb_Up)) {
			cursorState -= 1;
			if (cursorState == -1) {
				cursorState = 4;
			}
			gfx_FillRectangle(205, 35, 10, 70);
			gfx_PrintStringXY(">", 208, 35 + 15 * cursorState);
			Wait(20);
		}
		if (kb_Data[1] & kb_2nd) {
			gfx_SetDrawBuffer();
			if (cursorState == 0) {
				int chosenPokemon;
				choosePokemon:
				chosenPokemon = menu_PokemonMenu(true);
				if (chosenPokemon != 0) {
					menu_PokemonDetails(chosenPokemon - 1);
					goto choosePokemon;
				}
				return 0;
			}
			if (cursorState == 1) {
				int chosenItem;
				chooseItem:
				chosenItem = menu_Items(false);
				if (chosenItem != -1) {
					items_UseItem(chosenItem);
					goto chooseItem;
				}
				return 0;
			}
			if (cursorState == 2) {
				return 0;
			}
			if (cursorState == 3) {
				save_Save();
				return 0;
			}
			else if (cursorState == 4) {
				return 2;
			}
		}
	}
	gfx_SetDrawBuffer();
	return 0;
}

int menu_PokemonMenu(bool pressEnter) {
	int partyIndex, statusIndex, menuState;
	gfx_sprite_t *backgroundSprite;
	gfx_sprite_t *pokemonSprite;
	int baseX[6] = { 20,175,20,175,20,175 };
	int baseY[6] = { 20,20,70,70,120,120 };


	menuState = 0;
	backgroundSprite = gfx_MallocSprite(160, 120);
	pokemonSprite = gfx_MallocSprite(133, 36);
	zx7_Decompress(backgroundSprite, pokemonlist_compressed);
	zx7_Decompress(pokemonSprite, menupokemon_compressed);
	MallocIcons();


	menu_Setup();

	redraw_PokemonMenu:
	gfx_SetDrawBuffer();

	gfx_FillScreen(colors[1]);
	text_DrawTextBox();
	gfx_ScaledSprite_NoClip(backgroundSprite, 0, 0, 2, 2);
	for (partyIndex = 0; partyIndex < 6; partyIndex++) {
		int healthRatio;
		if (party[partyIndex].id != 0) {
			gfx_TransparentSprite_NoClip(pokemonSprite, baseX[partyIndex], baseY[partyIndex]);
			gfx_PrintStringXY(data_pokemon[party[partyIndex].id].name, baseX[partyIndex] + 4, baseY[partyIndex] + 4);
			sprintf(str, "Lv%u", party[partyIndex].level);
			gfx_PrintStringXY(str, baseX[partyIndex] + 92, baseY[partyIndex] + 4);

			if (party[partyIndex].currentstatus > 0) {
				gfx_TransparentSprite_NoClip(statusIcons[party[partyIndex].currentstatus - 1], baseX[partyIndex] + 7, baseY[partyIndex] + 14);
			}
			healthRatio = (party[partyIndex].currenthealth * 78) / stats_CalculateStats(party[partyIndex]).health;
			if (healthRatio > 34) {
				gfx_SetColor(colors[8]);
			}
			else if (healthRatio > 14) {
				gfx_SetColor(colors[9]);
			}
			else {
				gfx_SetColor(colors[10]);
			}
			gfx_FillRectangle(baseX[partyIndex] + 47, baseY[partyIndex] + 16, healthRatio, 4);

			sprintf(str, "%u/%u", party[partyIndex].currenthealth, stats_CalculateStats(party[partyIndex]).health);
			gfx_PrintStringXY(str, baseX[partyIndex] + 32, baseY[partyIndex] + 24);
		}
	}
	gfx_PrintStringXY("Select a pokemon", 30, 190);
	if (pressEnter) {
		gfx_PrintStringXY("Press Enter to move pokemon", 30, 205);
	}

	gfx_SetDrawScreen();

	gfx_Blit(gfx_buffer);
	gfx_TransparentSprite_NoClip(menucursor, baseX[menuState] - 15, baseY[menuState] + 8);

	


	Wait(20);
	kb_Scan();

	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	while (!(((kb_Data[1] & kb_2nd) && party[menuState].id != 0) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
		if ((kb_Data[6] & kb_Enter) && pressEnter) {
			tempcharacter = party[menuState];
			for (partyIndex = menuState; partyIndex > 0; partyIndex--) {
				party[partyIndex] = party[partyIndex - 1];
			}
			party[0] = tempcharacter;
			goto redraw_PokemonMenu;
		}
		if (kb_Data[7] & kb_Down) {
			menuState+=2;
			if (menuState == 6 || menuState == 7) {
				menuState -= 6;
			}
		}
		else if (kb_Data[7] & kb_Up) {
			menuState-=2;
			if (menuState == -1 || menuState == -2) {
				menuState += 6;
			}
		}
		else if ((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
			if (menuState % 2 == 0) {
				menuState += 1;
			}
			else {
				menuState -= 1;
			}
		}
		if ((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left) || (kb_Data[7] & kb_Down) || (kb_Data[7] & kb_Up)) {
			gfx_Blit(gfx_buffer);
			gfx_TransparentSprite_NoClip(menucursor, baseX[menuState] - 15, baseY[menuState] + 8);
			Wait(20);
		}
	}
	free(backgroundSprite);
	free(pokemonSprite);
	FreeIcons();
	gfx_SetDrawBuffer();
	if (kb_Data[6] & kb_Clear) {
		return 0;
	}
	return menuState + 1;
}
void menu_PokemonDetails(int pokemonIndex) {
	int page, menuState, tempMove, moveIndex;
	gfx_sprite_t *backgroundSprite;

	backgroundSprite = gfx_MallocSprite(160, 120);
	

	menu_Setup();

	page = 1;
	menuState = 0;
	while (page != 0) {
		Wait(20);
		kb_Scan();
		if (page == 1) {
			gfx_sprite_t *pokemonSprite;
			zx7_Decompress(backgroundSprite, infoscreen1_compressed);
			gfx_ScaledSprite_NoClip(backgroundSprite, 0, 0, 2, 2);


			gfx_SetTextScale(2, 2);


			
			gfx_PrintStringXY(data_pokemon[party[pokemonIndex].id].name, 4, 32);
			pokemonSprite = gfx_MallocSprite(56, 56);
			if (party[pokemonIndex].id < 80) {
				zx7_Decompress(pokemonSprite, PKMNSD0[party[pokemonIndex].id]);
			}
			else {
				zx7_Decompress(pokemonSprite, PKMNSD1[party[pokemonIndex].id - 80]);
			}
			gfx_ScaledTransparentSprite_NoClip(pokemonSprite, 26, 52, 2, 2);
			free(pokemonSprite);

			sprintf(str, "%u/%u", party[pokemonIndex].currenthealth, stats_CalculateStats(party[pokemonIndex]).health);
			gfx_PrintStringXY(str, 208, 38);
			sprintf(str, "%u", stats_CalculateStats(party[pokemonIndex]).attack);
			gfx_PrintStringXY(str, 228, 62);
			sprintf(str, "%u", stats_CalculateStats(party[pokemonIndex]).defence);
			gfx_PrintStringXY(str, 228, 86);
			sprintf(str, "%u", stats_CalculateStats(party[pokemonIndex]).specialattack);
			gfx_PrintStringXY(str, 228, 110);
			sprintf(str, "%u", stats_CalculateStats(party[pokemonIndex]).specialdefence);
			gfx_PrintStringXY(str, 228, 134);
			sprintf(str, "%u", stats_CalculateStats(party[pokemonIndex]).speed);
			gfx_PrintStringXY(str, 228, 158);

			sprintf(str, "%u", party[pokemonIndex].level);
			gfx_PrintStringXY(str, 64, 186);
			gfx_TransparentSprite_NoClip(typeIcons[data_pokemon[party[pokemonIndex].id].element1 - 1], 218, 187);
			if (data_pokemon[party[pokemonIndex].id].element2 != 0) {
				gfx_TransparentSprite_NoClip(typeIcons[data_pokemon[party[pokemonIndex].id].element2 - 1], 254, 187);
			}
			sprintf(str, "%i/%i", party[pokemonIndex].xp, xpPerLevel[data_pokemon[party[pokemonIndex].id].xptype][party[pokemonIndex].level + 1]);
			gfx_PrintStringXY(str, 64, 210);

			gfx_SetTextScale(1, 1);

			gfx_SwapDraw();
			while (!((kb_Data[7] & kb_Right) || (kb_Data[6] & kb_Clear))) {
				kb_Scan();
			}
		}
		else if (page == 2) {
			char description[19] = { 0 };
			zx7_Decompress(backgroundSprite, infoscreen2_compressed);
			gfx_ScaledSprite_NoClip(backgroundSprite, 0, 0, 2, 2);

			gfx_SetTextScale(2, 2);

			gfx_PrintStringXY(data_pokemon[party[pokemonIndex].id].name, 4, 32);
			gfx_TransparentSprite_NoClip(typeIcons[data_pokemon[party[pokemonIndex].id].element1 - 1], 164, 34);
			if (data_pokemon[party[pokemonIndex].id].element2 != 0) {
				gfx_TransparentSprite_NoClip(typeIcons[data_pokemon[party[pokemonIndex].id].element2 - 1], 200, 34);
			}

			gfx_TransparentSprite_NoClip(menucursor, 8, 56 + 24 * menuState);
			for (moveIndex = 0; moveIndex < 4; moveIndex++) {
				if (party[pokemonIndex].moves[moveIndex] > 0) {
					gfx_PrintStringXY(data_moves[party[pokemonIndex].moves[moveIndex]].name, 30, 58 + 24 * moveIndex);
					gfx_TransparentSprite_NoClip(typeIcons[data_moves[party[pokemonIndex].moves[moveIndex]].element - 1], 242, 60 + 24 * moveIndex);
					if(data_moves[party[pokemonIndex].moves[moveIndex]].type == 0)
						gfx_TransparentSprite_NoClip(categoryIcons[0], 280, 59 + 24 * moveIndex);
					else if (data_moves[party[pokemonIndex].moves[moveIndex]].type == 1) {
						gfx_TransparentSprite_NoClip(categoryIcons[1], 280, 59 + 24 * moveIndex);
					}
					else {
						gfx_TransparentSprite_NoClip(categoryIcons[2], 280, 59 + 24 * moveIndex);
					}
				}
			}

			if (party[pokemonIndex].moves[menuState] != 0) {
				gfx_SetTextScale(1, 1);
				strncpy(description, data_moveDescriptions[party[pokemonIndex].moves[menuState] - 1], 18);
				gfx_PrintStringXY(description, 170, 185);
				strncpy(description, data_moveDescriptions[party[pokemonIndex].moves[menuState] - 1] + 18, 18);
				gfx_PrintStringXY(description, 170, 194);
				strncpy(description, data_moveDescriptions[party[pokemonIndex].moves[menuState] - 1] + 36, 18);
				gfx_PrintStringXY(description, 170, 203);
				strncpy(description, data_moveDescriptions[party[pokemonIndex].moves[menuState] - 1] + 54, 18);
				gfx_PrintStringXY(description, 170, 212);
				strncpy(description, data_moveDescriptions[party[pokemonIndex].moves[menuState] - 1]+72, 6);
				gfx_PrintStringXY(description, 170, 221);
				gfx_SetTextScale(2, 2);
			}

			sprintf(str, "%u", data_moves[party[pokemonIndex].moves[menuState]].power);
			if (data_moves[party[pokemonIndex].moves[menuState]].power < 5) {
				sprintf(str, "-");
			}
			gfx_PrintStringXY(str, 66, 168);
			sprintf(str, "%u", data_moves[party[pokemonIndex].moves[menuState]].accuracy);
			gfx_PrintStringXY(str, 66, 194);
			sprintf(str, "%u/%u", party[pokemonIndex].pp[menuState], data_moves[party[pokemonIndex].moves[menuState]].uses);
			gfx_PrintStringXY(str, 66, 216);

			gfx_SetTextScale(1, 1);


			gfx_SwapDraw();
			while (!((kb_Data[7] & kb_Left) || (kb_Data[6] & kb_Clear) || (kb_Data[7] & kb_Up) || (kb_Data[7] & kb_Down) || (kb_Data[6] & kb_Enter) || (kb_Data[6] & kb_Sub) || (kb_Data[6] & kb_Add))) {
				kb_Scan();
			}
		}
		if (kb_Data[7] & kb_Right) {
			page = 2;
		}
		if (kb_Data[7] & kb_Left) {
			page = 1;
		}
		if (kb_Data[7] & kb_Up) {
			if (menuState > 0) {
				menuState--;
			}
		}
		if (kb_Data[7] & kb_Down) {
			if (menuState < 3) {
				menuState++;
			}
		}
		if ((kb_Data[6] & kb_Sub) || (kb_Data[6] & kb_Add) && page == 2) {
			if (kb_Data[6] & kb_Add) {
				party[pokemonIndex].moves[menuState]++;
			}
			else {
				party[pokemonIndex].moves[menuState]--;
			}
		}
		if ((kb_Data[6] & kb_Enter) && page == 2) {
			tempMove = party[pokemonIndex].moves[menuState];
			for (moveIndex = menuState; moveIndex > 0; moveIndex--) {
				party[pokemonIndex].moves[moveIndex] = party[pokemonIndex].moves[moveIndex - 1];
			}
			party[pokemonIndex].moves[0] = tempMove;
			menuState = 0;
		}
		if (kb_Data[6] & kb_Clear) {
			page = 0;
		}
	}
	free(backgroundSprite);
}
int menu_Items(bool inBattle) {
	int page, menuState, cursorState, itemIndex, usableItemCount, usableItems[20], usableTMCount, usableTMs[165];
	gfx_sprite_t *backgroundSprite;


	/* Generate Usable Items List */
	usableItemCount = 0;
	usableTMCount = 0;
	for (itemIndex = 0; itemIndex < 20; itemIndex++) {
		if (playerItems[itemIndex] > 0) {
			usableItems[usableItemCount] = itemIndex;
			usableItemCount++;
		}
	}
	for (itemIndex = 0; itemIndex < 165; itemIndex++) {
		if (playerItems[itemIndex + 20] > 0) {
			usableTMs[usableTMCount] = itemIndex + 20;
			usableTMCount++;
		}
	}
	backgroundSprite = gfx_MallocSprite(160, 88);
	zx7_Decompress(backgroundSprite, bag_compressed);

	menu_Setup();


	page = 1;
	menuState = 0;
	cursorState = 0;
	while (page != 0)
	{

		gfx_FillScreen(colors[1]);
		text_DrawTextBox();
		gfx_ScaledSprite_NoClip(backgroundSprite, 0, 0, 2, 2);
		gfx_TransparentSprite_NoClip(menucursorsmall, 132, 19 + 14 * cursorState);
		
		Wait(15);

		kb_Scan();
		if (page == 1) {
			char description[37] = { 0 };
			gfx_SetTextScale(2, 2);
			gfx_PrintStringXY("Items", 12, 19);
			gfx_SetTextScale(1, 1);
			for (itemIndex = menuState; itemIndex < menuState + 10 && itemIndex < usableItemCount; itemIndex++) {
				gfx_PrintStringXY(itemNames[usableItems[itemIndex]], 140, 21 + 14 * (itemIndex - menuState));
				sprintf(str, "%u", playerItems[usableItems[itemIndex]]);
				gfx_PrintStringXY(str, 274, 21 + 14 * (itemIndex - menuState));
			}

			strncpy(description, itemDescriptions[usableItems[menuState + cursorState]], 36);
			gfx_PrintStringXY(description, 14, 185);
			strncpy(description, itemDescriptions[usableItems[menuState + cursorState]] + 36, 32);
			gfx_PrintStringXY(description, 14, 200);

			gfx_SwapDraw();
			while (!((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left) || (kb_Data[6] & kb_Clear) || (kb_Data[1] & kb_2nd) || (kb_Data[7] & kb_Up) || (kb_Data[7] & kb_Down))) {
				kb_Scan();
			}
		}
		else if (page == 2) {
			char description[37] = { 0 };
			gfx_SetTextScale(2, 2);
			gfx_PrintStringXY("TMs", 12, 19);
			gfx_SetTextScale(1, 1);
			for (itemIndex = menuState; itemIndex < menuState + 10 && itemIndex < usableTMCount; itemIndex++) {
				sprintf(str, "TM%u %s", usableTMs[itemIndex] - 19, data_moves[usableTMs[itemIndex] - 19].name);
				gfx_PrintStringXY(str, 140, 21 + 14 * (itemIndex - menuState));
			}

			strncpy(description, data_moveDescriptions[usableTMs[menuState + cursorState] - 20], 36);
			gfx_PrintStringXY(description, 14, 185);
			strncpy(description, data_moveDescriptions[usableTMs[menuState + cursorState] - 20] + 36, 36);
			gfx_PrintStringXY(description, 14, 200);
			strncpy(description, data_moveDescriptions[usableTMs[menuState + cursorState] - 20] + 72, 6);
			gfx_PrintStringXY(description, 14, 215);

			gfx_SwapDraw();
			while (!((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left) || (kb_Data[6] & kb_Clear) || (kb_Data[1] & kb_2nd) || (kb_Data[7] & kb_Up) || (kb_Data[7] & kb_Down))) {
				kb_Scan();
			}
		}
		if (kb_Data[7] & kb_Up) {
			if (menuState > 0 && cursorState <= 2) {
				menuState--;
			}
			else if(menuState + cursorState > 0) {
				cursorState--;
			}
		}
		if (kb_Data[7] & kb_Down) {
			if (page == 1) {
				if (menuState + 10 < usableItemCount && cursorState >= 3) {
					menuState++;
				}
				else if (menuState + cursorState < usableItemCount - 1) {
					cursorState++;
				}
			}
			else {
				if (menuState + 10 < usableTMCount && cursorState >= 7) {
					menuState++;
				}
				else if (menuState + cursorState < usableTMCount - 1) {
					cursorState++;
				}
			}
		}
		if (((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) && !inBattle) {
			if (page == 1) {
				page = 2;
			}
			else {
				page = 1;
			}
			menuState = 0;
			cursorState = 0;
		}
		if (kb_Data[6] & kb_Clear) {
			page = 0;
		}
		if (kb_Data[1] & kb_2nd) {
			int itemToUse;
			if (page == 1) {
				itemToUse = usableItems[menuState + cursorState];
			}
			else {
				itemToUse = usableTMs[menuState + cursorState];
			}
			free(backgroundSprite);
			gfx_SetDrawBuffer();
			return itemToUse;
		}
	}
	free(backgroundSprite);
	gfx_SetDrawBuffer();
	return -1;
}
void menu_Box() {
	/*int pokemonIndex;
	gfx_sprite_t *backgroundSprite;
	gfx_sprite_t *pokemonSprites[30];
	gfx_sprite_t *largeSprite;

	backgroundSprite = gfx_MallocSprite(160, 120);


	menu_Setup();

	largeSprite = gfx_MallocSprite(56, 56);
	for (pokemonIndex = 0; pokemonIndex < 30; pokemonIndex++) {
		pokemonSprites[pokemonIndex] = largeSprite = gfx_MallocSprite(28, 28);
		if (party[pokemonIndex].id < 80) {
			zx7_Decompress(largeSprite, PKMNSD0[party[pokemonIndex].id]);
		}
		else {
			zx7_Decompress(largeSprite, PKMNSD1[party[pokemonIndex].id - 80]);
		}
		memcpy(pokemonSprites[pokemonIndex] + 2, largeSprite + 2, 784);
		gfx_TransparentSprite_NoClip(pokemonSprites[pokemonIndex], 30 * (pokemonIndex % 6), 30 * (pokemonIndex / 6));
	}


	while (!((kb_Data[6] & kb_Clear))) {
		kb_Scan();
	}

	free(backgroundSprite);*/
}