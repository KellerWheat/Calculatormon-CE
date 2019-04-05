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
#include <debug.h>

#include "map.h"
#include "battle.h"
#include "menu.h"
#include "misc.h"
#include "data.h"
#include "items.h"
#include "text.h"
#include "savegame.h"
#include "stats.h"
#include "gfx/map_gfx.h"
#include "items.h"

#include "gfx/PKMNSD12.h"
#include "gfx/PKMNSD13.h"

#define OUTDOORWIDTH 48
#define OUTDOORHEIGHT 40
#define MAX_X ((OUTDOORWIDTH - 21)*16) // (Tilemap Width - Draw Width) * 16
#define MAX_Y ((OUTDOORHEIGHT - 15)*16) // (Tilemap Height - Draw Height) * 16

void HealParty(void);
void OpenBox(void);
void TalkToNpc1(void);
void TalkToNpc2(void);
void EnterDoor(uint8_t index);
void ExitBuilding(void);
void ExitZone(uint8_t index);
void FightTrainer(uint8_t index);

int32_t Clamp(int32_t number, int min, int max);
/* Gets the tile the player is facing*/
uint8_t GetNextTile(uint8_t tx, uint8_t ty, int width);
/* Gets the typemap at a specific tile */
uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width);
void DrawEnemies(void);

const int battlechance = 10; /* Chance for enemy to spawn out of 100 */

const uint8_t playertilemap[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
};
const uint8_t enemytilemap[] = {
	0,1,2,3,4,5,6,7
};
uint8_t pokeballtilemap[] = {
	0,0,0,0,0,0
};

int32_t screenX = 0;
int32_t screenY = 0;

uint8_t tx; /* x tile */
uint8_t ty; /* y tile */

uint8_t nextTile;
uint8_t moveState = 0;
uint8_t moveDir = 3;
uint8_t playerState = 0;

gfx_tilemap_t tilemap;
gfx_tilemap_t playermap;
gfx_tilemap_t enemymap;
gfx_tilemap_t pokeballmap;

uint8_t npc1;
uint8_t npc2;

uint8_t currentTrainer = 0; /* 0-15 */
struct pokemonData tempcharacter2;
const struct pokemonData clearcharacter2 = {0};

char str1[20];

void map_Initialize(void) {
	PKMNSD12_init();
	PKMNSD13_init();

	/* Initialize the tilemap */
	tilemap.map = data_tileMaps[currentZone];
	tilemap.tiles = tileset_tiles;
	tilemap.type_width = gfx_tile_16_pixel;
	tilemap.type_height = gfx_tile_16_pixel;
	tilemap.tile_height = 16;
	tilemap.tile_width = 16;
	tilemap.draw_height = 15;
	tilemap.draw_width = 21;
	tilemap.height = OUTDOORHEIGHT;
	tilemap.width = OUTDOORWIDTH;
	tilemap.y_loc = 8;
	tilemap.x_loc = -8;

	/* Initialize player tilemap */
	playermap.map = playertilemap;
	playermap.tiles = player_tiles;
	playermap.type_width = gfx_tile_16_pixel;
	playermap.type_height = gfx_tile_16_pixel;
	playermap.tile_height = 16;
	playermap.tile_width = 16;
	playermap.draw_height = 2;
	playermap.draw_width = 1;
	playermap.height = 2;
	playermap.width = 12;
	playermap.y_loc = 0;
	playermap.x_loc = 0;

	/* Initialize tilemap for other trainers */
	enemymap.map = enemytilemap;
	enemymap.tiles = enemy_tiles;
	enemymap.type_width = gfx_tile_16_pixel;
	enemymap.type_height = gfx_tile_16_pixel;
	enemymap.tile_height = 16;
	enemymap.tile_width = 16;
	enemymap.draw_height = 2;
	enemymap.draw_width = 1;
	enemymap.height = 2;
	enemymap.width = 4;
	enemymap.y_loc = 0;
	enemymap.x_loc = 0;

	/* Initialize Pokeball Tilemap (shows which pokemon have fainted)*/
	pokeballmap.map = pokeballtilemap;
	pokeballmap.tiles = pokeball_tiles;
	pokeballmap.type_width = gfx_tile_16_pixel;
	pokeballmap.type_height = gfx_tile_16_pixel;
	pokeballmap.tile_height = 16;
	pokeballmap.tile_width = 16;
	pokeballmap.draw_height = 1;
	pokeballmap.draw_width = 6;
	pokeballmap.height = 1;
	pokeballmap.width = 3;
	pokeballmap.y_loc = 0;
	pokeballmap.x_loc = 0;

	if (indoors) {
		tilemap.width = 21;
		tilemap.height = 15;
		tilemap.map = data_buildingTileMaps[currentBuilding];
	}

	tx = playerX / 16;
	ty = playerY / 16;
}
void map_Setup(void) {
	int pokemonIndex;
	/* Generate pokemon info To Display At Top of Screen */
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {
		if (party[pokemonIndex].id == 0) {
			pokeballtilemap[pokemonIndex] = 2;
		}
		else {
			if (party[pokemonIndex].currenthealth > 0) {
				pokeballtilemap[pokemonIndex] = 0;
			}
			else {
				pokeballtilemap[pokemonIndex] = 1;
			}
		}
	}


	tx = playerX / 16;
	ty = playerY / 16;
	gfx_SetPalette(map_gfx_pal, sizeof_map_gfx_pal, 0);
	SetColors(0);
	gfx_SetTextScale(1, 1);
	gfx_SetTextFGColor(colors[1]);

}
int map_Loop(void) {
	/* Open Menu */
	if (kb_Data[6] & kb_Enter) {
		Wait(20);
		return 2;
	}
	/* If player presses 2nd */
	if ((kb_Data[1] & kb_2nd)) {
		nextTile = GetNextTile(tx, ty, tilemap.width);
		if (nextTile == 0x41) {
			HealParty();
		}
		else if (nextTile == 0x42) {
			items_ItemShop();
		}
		else if (nextTile == 0x43) {
			OpenBox();
		}
		else if (nextTile == 0x44) {
			TalkToNpc1();
		}
		else if (nextTile == 0x45) {
			TalkToNpc2();
		}
		else if (nextTile >= 0x50 && nextTile < 0x60) {
			EnterDoor(nextTile - 80);
		}
		else if (nextTile >= 0x60 && nextTile < 0x70) {
			FightTrainer(nextTile - 96);
			return 1;
		}
	}
	/* If moving */
	if (moveState > 0) {
		moveState--;
		//Wait(1);
		if (moveDir == 1) {
			playerX += 2;
		}
		if (moveDir == 2) {
			playerX -= 2;
		}
		if (moveDir == 3) {
			playerY += 2;
		}
		if (moveDir == 4) {
			playerY -= 2;
		}

		if (moveState > 5) {
			playerState = 1;
		}
		else if (moveState > 3) {
			playerState = 0;
		}
		else if (moveState > 1) {
			playerState = 2;
		}
		else if (moveState > 0) {
			playerState = 0;
		}

		if (moveState == 0) {
			/* Arrive At Tile*/
			tx = playerX / 16;
			ty = playerY / 16;
			nextTile = GetTypeMapData(tx, ty, tilemap.width);
			if (nextTile >= 0x10 && nextTile < 0x20) {
				/* Grass */
				if ((rand() % 100) <= battlechance) {
					uint8_t wildSpawn;
					wildSpawn = rand() % 5;
					battle_SpawnWild(data_zoneData[currentZone].spawnids[wildSpawn][nextTile -16], data_zoneData[currentZone].spawnminlevels[wildSpawn][nextTile - 16], data_zoneData[currentZone].spawnmaxlevels[wildSpawn][nextTile - 16]);
					return 1;
				}
			}
			else if (nextTile == 0x01) {
				ExitBuilding();
			}
			else if (nextTile >= 0x20 && nextTile < 0x30) {
				ExitZone(nextTile - 32);
			}
			else if (nextTile >= 0x30 && nextTile < 0x40) {
				FightTrainer(nextTile - 48);
				return 1;
			}
		}
	}

	/* If not moving */
	else {
		if (kb_Data[7] & kb_Right) {
			moveDir = 1;
		}
		else if (kb_Data[7] & kb_Left) {
			moveDir = 2;
		}
		else if (kb_Data[7] & kb_Down) {
			moveDir = 3;

		}
		else if (kb_Data[7] & kb_Up) {
			moveDir = 4;
		}
		if (kb_Data[7]) {
			if (GetNextTile(tx, ty, tilemap.width) < 64) {
				moveState = 8;
			}
			if (GetNextTile(tx, ty, tilemap.width) >= 112 && GetNextTile(tx, ty, tilemap.width) < 120) {
				if (GetNextTile(tx, ty, tilemap.width) - 111 > badgeCount) {
					sprintf(str, "You need %u badges to pass", (GetNextTile(tx, ty, tilemap.width) - 111));
					text_Display(str, true);
				}
				else {
					moveState = 8;
				}
			}
		}
	}

	/* Move Screen and Redraw */
	if (indoors) {
		screenX = 0;
		screenY = 0;
		gfx_Tilemap(&tilemap, screenX, screenY);

		playermap.x_loc = (playerX - 8);
		playermap.y_loc = (playerY - 8);
		gfx_TransparentTilemap(&playermap, 16 * (moveDir * 3 + playerState - 3), 0);
	}
	else {
		screenX = Clamp(playerX - 160, 0, MAX_X);
		screenY = Clamp(playerY - 112, 0, MAX_Y);

		gfx_Tilemap(&tilemap, screenX, screenY);

		playermap.x_loc = (playerX - screenX - 8);
		playermap.y_loc = (playerY - screenY - 8);
		gfx_TransparentTilemap(&playermap, 16 * (moveDir * 3 + playerState - 3), 0);
	}
	DrawEnemies();
	
	map_DrawInformationBar();
	gfx_SwapDraw();
	return 0;
}

void HealParty(void) {
	int pokemonIndex, statusIndex;
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {
		party[pokemonIndex].currenthealth = stats_CalculateStats(party[pokemonIndex]).health;
		for (statusIndex = 0; statusIndex < 5; statusIndex++) {
			party[pokemonIndex].currentstatus[statusIndex] = 0;
		}
	}
	text_Display("Your party has been healed", true);
}
void OpenBox(void) {
	uint8_t boxMode;
	boxMode = text_AskQuestion2("Box", "Delete Save", false);
	if (boxMode == 2) {
		text_Display("Are You Sure?", false);
		if (text_AskQuestion2("Yes", "No", false) == 1) {
			ti_Delete(appVarName);
		}
	}
	else if (boxMode == 1) {
		uint8_t lastSelection = 255;
		uint8_t boxPage = 0;
		uint8_t boxCurrent = 0;
		int boxIndex;

	redrawbox:
		gfx_SetColor(colors[0]);
		gfx_FillRectangle(20, 20, 280, 200);
		gfx_SetColor(colors[1]);
		gfx_Rectangle(20, 20, 280, 200);
		gfx_SetColor(colors[0]);

		gfx_PrintStringXY(">", 25, 25 + 20 * (boxCurrent));
		for (boxIndex = 0; boxIndex < 10 && boxIndex + boxPage < 36; boxIndex++) {
			if (party[boxIndex + boxPage].id > 0) {
				sprintf(str, "LV%u %s", party[boxIndex + boxPage].level, data_pokemon[party[boxIndex + boxPage].id].name);
				gfx_PrintStringXY(str, 35, 25 + boxIndex * 20);
			}
		}
		gfx_SwapDraw();
		Wait(20);

		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		kb_Scan();
		while (!(kb_Data[6] & kb_Clear)) {
			kb_Scan();
			if (kb_Data[1] & kb_2nd) {
				if (lastSelection == 255) {
					lastSelection = boxCurrent + boxPage;
				}
				else {
					tempcharacter2 = party[lastSelection];
					party[lastSelection] = party[boxCurrent + boxPage];
					party[boxCurrent + boxPage] = tempcharacter2;
					lastSelection = 255;
				}
				goto redrawbox;
			}
			if (kb_Data[6] & kb_Enter) {
				text_Display("Release Pokemon?", false);
				if (text_AskQuestion2("Yes", "No", false) == 1) {
					party[boxCurrent + boxPage] = clearcharacter2;
				}
				goto redrawbox;
			}
			if ((kb_Data[7] & kb_Down) && boxCurrent + boxPage < 36) {
				boxCurrent++;
				if (boxCurrent == 6 && boxPage < 26) {
					boxPage++;
					boxCurrent--;
				}
				goto redrawbox;
			}
			if ((kb_Data[7] & kb_Up) && boxCurrent != 0) {
				boxCurrent--;
				if (boxCurrent == 4 && boxPage > 0) {
					boxPage--;
					boxCurrent++;
				}
				goto redrawbox;
			}
		}
	}
}
void TalkToNpc1(void) {
	text_Display(data_npcText[npc1], false);
	if (!takenGifts[npc1] && data_npcReward[npc1] > 0) {
		playerItems[data_npcReward[npc1] - 1]++;
		items_IndexToName(str1, data_npcReward[npc1] - 1);
		if (data_npcReward[npc1] - 1 >= 20) {
			sprintf(str, "Recieved the TM for %s", str1);
		}
		else {
			sprintf(str, "Recieved a %s", str1);
		}
		text_Display(str, false);
		takenGifts[npc1] = true;
	}
}
void TalkToNpc2(void) {
	text_Display(data_npcText[npc2], false);
	if (!takenGifts[npc2]) {
		playerItems[data_npcReward[npc2] - 1]++;
		items_IndexToName(str1, data_npcReward[npc2] - 1);
		if (data_npcReward[npc2] - 1 >= 20) {
			sprintf(str, "Recieved the TM for %s", str1);
		}
		else {
			sprintf(str, "Recieved a %s", str1);
		}
		text_Display(str, false);
		takenGifts[npc2] = true;
	}
}
void EnterDoor(uint8_t index) {
	int tileIndex;
	lastPlayerX = playerX;
	lastPlayerY = playerY;
	indoors = true;
	currentBuilding = data_zoneData[currentZone].doorzones[index];

	for (tileIndex = 0; tileIndex < tileset_tiles_num; tileIndex++) {
		memcpy(tileset_tiles_data[tileIndex], indoortileset_tiles_data[tileIndex], 258);
	}
	tilemap.width = 21;
	tilemap.height = 15;
	tilemap.map = data_buildingTileMaps[currentBuilding];
	playerX = data_zoneData[currentZone].doorx[index] * 16;
	playerY = data_zoneData[currentZone].doory[index] * 16;
	tx = playerX / 16;
	ty = playerY / 16;
	npc1 = data_zoneData[currentZone].doornpc1[index];
	npc2 = data_zoneData[currentZone].doornpc2[index];
}
void ExitBuilding(void) {
	int tileIndex;
	playerX = lastPlayerX;
	playerY = lastPlayerY;
	indoors = false;
	for (tileIndex = 0; tileIndex < tileset_tiles_num; tileIndex++) {
		memcpy(tileset_tiles_data[tileIndex], outdoortileset_tiles_data[tileIndex], 258);
	}
	tilemap.width = OUTDOORWIDTH;
	tilemap.height = OUTDOORHEIGHT;
	tx = playerX / 16;
	ty = playerY / 16;
	tilemap.map = data_tileMaps[currentZone];
}
void ExitZone(uint8_t index) {
	playerX = data_zoneData[currentZone].exitx[index] * 16;
	playerY = data_zoneData[currentZone].exity[index] * 16;
	currentZone = data_zoneData[currentZone].exitzone[index];
	tilemap.map = data_tileMaps[currentZone];
	tx = playerX / 16;
	ty = playerY / 16;
}
void FightTrainer(uint8_t index) {
	currentTrainer = index;
	if (indoors && !defeatedTrainersIndoors[currentBuilding][currentTrainer]) {
		text_Display(data_trainerText[data_buildingZoneData[currentBuilding].data_trainerText[index]], true);
		battle_SpawnTrainer(data_buildingZoneData[currentBuilding].trainerspawnids, data_buildingZoneData[currentBuilding].trainerspawnlevels, index);
	}
	else if (!indoors && !defeatedTrainers[currentZone][currentTrainer]) {
		text_Display(data_trainerText[data_zoneData[currentZone].data_trainerText[index]], true);
		battle_SpawnTrainer(data_zoneData[currentZone].trainerspawnids, data_zoneData[currentZone].trainerspawnlevels, index);
	}
}
int32_t Clamp(int32_t number, int min, int max) {
	if (number < min) {
		number = min;
	}
	if (number > max) {
		number = max;
	}
	return number;
}
uint8_t GetNextTile(uint8_t tx, uint8_t ty, int width) {

	switch (moveDir) {
	case 1:
		return(GetTypeMapData(tx + 1, ty, width));
	case 2:
		return(GetTypeMapData(tx - 1, ty, width));
	case 3:
		return(GetTypeMapData(tx, ty + 1, width));
	case 4:
		return(GetTypeMapData(tx, ty - 1, width));
	}
	return 0;
}

uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width) {
	if (indoors) {
		return (data_buildingTypeMaps[currentBuilding][(tx)+(ty)* width]);
	}
	else {
		return (data_typeMaps[currentZone][(tx)+(ty)* width]);
	}
}

void DrawEnemies(void) {
	uint32_t xloc = 0;
	uint32_t yloc = 0;
	for (i = 0; i < 16; i++) {
		if (indoors){
			if (data_buildingZoneData[currentBuilding].trainerdir[i] != 0) {
				xloc = data_buildingZoneData[currentBuilding].trainerx[i] * 16 - 8;
				yloc = data_buildingZoneData[currentBuilding].trainery[i] * 16 - 8;
				enemymap.x_loc = (xloc);
				enemymap.y_loc = (yloc);
				gfx_TransparentTilemap(&enemymap, 16 * (data_buildingZoneData[currentBuilding].trainerdir[i] - 1), 0);
			}
		}
		else {
			if (data_zoneData[currentZone].trainerdir[i] != 0) {
				xloc = data_zoneData[currentZone].trainerx[i] * 16 - screenX - 8;
				yloc = data_zoneData[currentZone].trainery[i] * 16 - screenY - 8;
				if (xloc < 336 && yloc < 240 && xloc > 0 && yloc > 0) {
				enemymap.x_loc = (xloc);
				enemymap.y_loc = (yloc);
				gfx_TransparentTilemap(&enemymap, 16 * (data_zoneData[currentZone].trainerdir[i] - 1), 0);
				}
			}
		}
		
	}
}

void map_DrawInformationBar(void) {
	gfx_SetColor(colors[0]);
	gfx_FillRectangle(0, 0, 320, 16);
	gfx_TransparentTilemap(&pokeballmap,0,0);
	sprintf(str, "%u$", playerMoney);
	gfx_PrintStringXY(str, 100, 5);
}

void map_Respawn(void) {
	for (i = 0; i < 6; i++) {
		party[i].currenthealth = stats_CalculateStats(party[i]).health;
		for (e = 0; e < 5; e++) {
			party[i].currentstatus[e] = 0;
		}
	}
	playerX = 6*16;
	playerY = 9*16;
	indoors = false;
	currentZone = 0;
	moveDir = 3;
	tilemap.width = OUTDOORWIDTH;
	tilemap.height = OUTDOORHEIGHT;
	tx = playerX / 16;
	ty = playerY / 16;
	tilemap.map = data_tileMaps[currentZone];
}