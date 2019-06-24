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
#include <compression.h>
#include <fileioc.h>

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

#include "gfx/PKMNSD4.h"

#define OUTDOORWIDTH 48
#define OUTDOORHEIGHT 40
#define MAX_X ((OUTDOORWIDTH - 21)*16)
#define MAX_Y ((OUTDOORHEIGHT - 15)*16)

void HealParty(void);
void OpenBox(void);
void TalkToNpc1(void);
void TalkToNpc2(void);
void EnterDoor(uint8_t index);
void ExitBuilding(void);
void ExitZone(uint8_t index);
bool FightTrainer(uint8_t index);

int32_t Clamp(int32_t number, int min, int max);
/* Gets the tile the player is facing*/
uint8_t GetNextTile(uint8_t tx, uint8_t ty, int width);
/* Gets the typemap at a specific tile */
uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width);
/* Draw 16 enemy trainers */
void DrawEnemies(void);
/* Loads the tilemap, typemap ,and zonedata for current area */
void LoadMap(void);

const int battlechance = 10; /* Chance for enemy to spawn out of 100 */


uint8_t pokeballdata[] = {
	0,0,0,0,0,0
};

int32_t screenX = 0;
int32_t screenY = 0;

uint8_t tx; /* x tile */
uint8_t ty; /* y tile */
bool running;
bool surfing;

uint8_t nextTile;
uint8_t moveState = 0;
uint8_t moveDir = 3;
uint8_t playerState = 0;

gfx_tilemap_t tilemap;

gfx_sprite_t *mapTiles[128];
gfx_sprite_t *playerSprites[20];
gfx_sprite_t *enemySprites[4];
gfx_sprite_t *pokeballSprites[3];
gfx_sprite_t *pauseMenuSprite;

uint8_t currentTileMap[1920];
uint8_t currentTypeMap[1920];
struct zoneData currentZoneData;
/* zoneData is in header file because battle.c needs it*/

uint8_t npc1;
uint8_t npc2;

uint8_t currentTrainer = 0; /* 0-15 */
struct pokemonData tempcharacter2;
const struct pokemonData clearcharacter2 = {0};

char str1[20];

void map_Initialize(void) {
	PKMNSD4_init();

	gfx_SetPalette(map_gfx_pal, sizeof_map_gfx_pal, 0);
	SetColors(0);
	gfx_SetTextFGColor(colors[1]);
	zx7_Decompress(textBoxSprite1, maptextbox1_compressed);
	zx7_Decompress(textBoxSprite2, maptextbox2_compressed);
	save_SelectSave();
	save_Load();

	/* Initialize the tilemap */
	tilemap.map = currentTileMap;
	tilemap.tiles = mapTiles;
	tilemap.type_width = gfx_tile_16_pixel;
	tilemap.type_height = gfx_tile_16_pixel;
	tilemap.tile_height = 16;
	tilemap.tile_width = 16;
	tilemap.draw_height = 16;
	tilemap.draw_width = 22;
	tilemap.height = OUTDOORHEIGHT;
	tilemap.width = OUTDOORWIDTH;
	tilemap.y_loc = 8;
	tilemap.x_loc = -8;

	if (indoors) {
		tilemap.width = 21;
		tilemap.height = 15;
	}

	tx = playerX / 16;
	ty = playerY / 16;
	LoadMap();
}
void map_Setup(void) {
	map_SetupGfx();

	tx = playerX / 16;
	ty = playerY / 16;
}

void map_SetupGfx(void) {
	int tileIndex = 0;
	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		mapTiles[tileIndex] = gfx_MallocSprite(16, 16);
		if (indoors) {
			zx7_Decompress(mapTiles[tileIndex], indoortileset_tiles_compressed[tileIndex]);
		}
		else {
			zx7_Decompress(mapTiles[tileIndex], outdoortileset_tiles_compressed[tileIndex]);
		}
	}
	for (tileIndex = 0; tileIndex < 12; tileIndex++) {
		playerSprites[tileIndex] = gfx_MallocSprite(16, 20);
		zx7_Decompress(playerSprites[tileIndex], PKMNSD4[0 + tileIndex]);
	}
	for (tileIndex = 12; tileIndex < 20; tileIndex++) {
		playerSprites[tileIndex] = gfx_MallocSprite(22, 26);
		zx7_Decompress(playerSprites[tileIndex], PKMNSD4[0 + tileIndex]);
	}
	for (tileIndex = 0; tileIndex < 4; tileIndex++) {
		enemySprites[tileIndex] = gfx_MallocSprite(16, 20);
		zx7_Decompress(enemySprites[tileIndex], PKMNSD4[20 + tileIndex]);
	}
	for (tileIndex = 0; tileIndex < 3; tileIndex++) {
		pokeballSprites[tileIndex] = gfx_MallocSprite(16, 16);
		zx7_Decompress(pokeballSprites[tileIndex], pokeball_tiles_compressed[tileIndex]);
	}
	pauseMenuSprite = gfx_MallocSprite(112, 96);
	zx7_Decompress(pauseMenuSprite, pausemenu_compressed);
	zx7_Decompress(textBoxSprite1, maptextbox1_compressed);
	zx7_Decompress(textBoxSprite2, maptextbox2_compressed);
	gfx_SetPalette(map_gfx_pal, sizeof_map_gfx_pal, 0);
	SetColors(0);
	gfx_SetTextScale(1, 1);
	gfx_SetTextFGColor(colors[1]);
	map_LoadPokeballs();
}

int map_Loop(void) {
	/* Open Menu */
	if (kb_Data[6] & kb_Enter) {
		int menuReturn;
		Wait(20);
		gfx_Blit(gfx_screen);
		gfx_Sprite(pauseMenuSprite, 195, 25);
		map_End();

		menuReturn = menu_Menu();
		map_SetupGfx();
		map_LoadPokeballs();
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return menuReturn;
	}
	/* If player presses 2nd */
	if ((kb_Data[1] & kb_2nd)) {
		nextTile = GetNextTile(tx, ty, tilemap.width);
		if (nextTile >= 0x10 && nextTile < 0x20 && (GetTypeMapData(tx, ty, tilemap.width) == 2)) {
			if (moveDir == 1) {
				playerX += 16;
			}
			if (moveDir == 2) {
				playerX -= 16;
			}
			if (moveDir == 3) {
				playerY += 16;
			}
			if (moveDir == 4) {
				playerY -= 16;
			}
			surfing = true;
			tx = playerX / 16;
			ty = playerY / 16;
		}
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
			if (FightTrainer(nextTile - 96)) {
				return 1;
			}
		}
		while ((kb_Data[1] & kb_2nd))
		{
			kb_Scan();
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
		if (surfing) {
			if (moveState > 3) {
				playerState = 1;
			}
			else {
				playerState = 0;
			}
		}
		else {
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
					battle_SpawnWild(currentZoneData.spawnids[wildSpawn][nextTile -16], currentZoneData.spawnminlevels[wildSpawn][nextTile - 16], currentZoneData.spawnmaxlevels[wildSpawn][nextTile - 16]);
					return 1;
				}
			}
			else if (nextTile == 0x01) {
				ExitBuilding();
			}
			else if (nextTile == 0x02) {
				surfing = false;
			}
			else if (nextTile >= 0x20 && nextTile < 0x30) {
				ExitZone(nextTile - 32);
			}
			else if (nextTile >= 0x30 && nextTile < 0x40) {
				if (FightTrainer(nextTile - 48)) {
					return 1;
				}
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
			if (kb_Data[2] & kb_Alpha) {
				running = true;
			}
			if (GetNextTile(tx, ty, tilemap.width) < 64 && !((GetTypeMapData(tx, ty, tilemap.width) == 2) && (GetNextTile(tx, ty, tilemap.width) >= 0x10) && (GetNextTile(tx, ty, tilemap.width) <= 0x20))) {
				moveState = 8;
			}
			if (GetNextTile(tx, ty, tilemap.width) >= 112 && GetNextTile(tx, ty, tilemap.width) < 120) {
				if (GetNextTile(tx, ty, tilemap.width) - 111 > badgeCount) {
					sprintf(str, "You need %u badges to pass", (GetNextTile(tx, ty, tilemap.width) - 111));
					text_Display("This route is closed. Come back later.", true);
				}
				else {
					moveState = 8;
				}
			}
		}
		else {
			running = false;
		}
	}

	map_Draw();

	gfx_SwapDraw();
	return 0;
}
void map_End(void) {
	int tileIndex = 0;
	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		free(mapTiles[tileIndex]);
	}
	for (tileIndex = 0; tileIndex < 20; tileIndex++) {
		free(playerSprites[tileIndex]);
	}
	for (tileIndex = 0; tileIndex < 4; tileIndex++) {
		free(enemySprites[tileIndex]);
	}
	for (tileIndex = 0; tileIndex < 3; tileIndex++) {
		free(pokeballSprites[tileIndex]);
	}
	free(pauseMenuSprite);
}

void map_Draw(void) {
	/* Move Screen and Redraw */
	if (indoors) {
		screenX = 0;
		screenY = 0;

		gfx_Tilemap(&tilemap, screenX, screenY);

		gfx_TransparentSprite_NoClip(playerSprites[moveDir * 3 + playerState - 3], playerX - 8, playerY + 4);
	}
	else {
		screenX = Clamp(playerX - 160, 0, MAX_X);
		screenY = Clamp(playerY - 112, 0, MAX_Y);

		gfx_Tilemap(&tilemap, screenX, screenY);
		
		gfx_TransparentSprite_NoClip(playerSprites[(12*surfing) + (moveDir - 1) * (3 - surfing) + playerState], playerX - screenX - 8, playerY - screenY + 4);
	}
	DrawEnemies();
	map_DrawInformationBar();
}

void HealParty(void) {
	int pokemonIndex, moveIndex;
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {
		party[pokemonIndex].currenthealth = stats_CalculateStats(party[pokemonIndex]).health;
		party[pokemonIndex].currentstatus = 0;
		for (moveIndex = 0; moveIndex < 4; moveIndex++) {
			party[pokemonIndex].pp[moveIndex] = data_moves[party[pokemonIndex].moves[moveIndex]].uses;
		}
	}
	text_Display("Your party has been healed", false);
	map_LoadPokeballs();
}
void OpenBox(void) {
	map_End();

	menu_Box();

	map_SetupGfx();
	map_LoadPokeballs();
	/*uint8_t boxMode;
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
	}*/
	/*map_LoadPokeballs();*/
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
	currentBuilding = currentZoneData.doorzones[index];

	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		zx7_Decompress(mapTiles[tileIndex], indoortileset_tiles_compressed[tileIndex]);
	}
	tilemap.width = 21;
	tilemap.height = 15;
	playerX = currentZoneData.doorx[index] * 16;
	playerY = currentZoneData.doory[index] * 16;
	tx = playerX / 16;
	ty = playerY / 16;
	npc1 = currentZoneData.doornpc1[index];
	npc2 = currentZoneData.doornpc2[index];
	LoadMap();
}
void ExitBuilding(void) {
	int tileIndex;
	playerX = lastPlayerX;
	playerY = lastPlayerY;
	indoors = false;
	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		zx7_Decompress(mapTiles[tileIndex], outdoortileset_tiles_compressed[tileIndex]);
	}
	tilemap.width = OUTDOORWIDTH;
	tilemap.height = OUTDOORHEIGHT;
	tx = playerX / 16;
	ty = playerY / 16;
	LoadMap();
}
void ExitZone(uint8_t index) {
	playerX = currentZoneData.exitx[index] * 16;
	playerY = currentZoneData.exity[index] * 16;
	currentZone = currentZoneData.exitzone[index];
	tx = playerX / 16;
	ty = playerY / 16;
	LoadMap();
}
bool FightTrainer(uint8_t index) {
	currentTrainer = index;
	if (indoors && !defeatedTrainersIndoors[currentBuilding][currentTrainer]) {
		text_Display(data_trainerText[currentZoneData.data_trainerText[index]], true);
		battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
		return true;
	}
	else if (!indoors && !defeatedTrainers[currentZone][currentTrainer]) {
		text_Display(data_trainerText[currentZoneData.data_trainerText[index]], true);
		battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
		return true;
	}
	return false;
}

void map_LoadPokeballs(void) {
	int pokemonIndex;
	/* Generate pokemon info To Display At Top of Screen */
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {
		if (party[pokemonIndex].id == 0) {
			pokeballdata[pokemonIndex] = 2;
		}
		else {
			if (party[pokemonIndex].currenthealth > 0) {
				pokeballdata[pokemonIndex] = 0;
			}
			else {
				pokeballdata[pokemonIndex] = 1;
			}
		}
	}
}
void LoadMap(void) {
	ti_var_t mapAV;
	if (indoors) {
		mapAV = ti_Open("PKMNMD1", "r");
		ti_Seek(((int)currentBuilding) * 1270, 0, mapAV);
		ti_Read(&currentTileMap, 315, 1, mapAV);
		ti_Read(&currentTypeMap, 315, 1, mapAV);
		ti_Read(&currentZoneData, 640, 1, mapAV);
	}
	else {
		mapAV = ti_Open("PKMNMD0", "r");
		ti_Seek(((int)currentZone) * 4480, 0, mapAV);
		ti_Read(&currentTileMap, 1920, 1, mapAV);
		ti_Read(&currentTypeMap, 1920, 1, mapAV);
		ti_Read(&currentZoneData, 640, 1, mapAV);
	}
	ti_CloseAll();
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
		return (currentTypeMap[(tx)+(ty)* width]);
	}
	else {
		return (currentTypeMap[(tx)+(ty)* width]);
	}
}

void DrawEnemies(void) {
	int xloc = 0;
	int yloc = 0;
	for (i = 0; i < 16; i++) {
		if (indoors){
			if (currentZoneData.trainerdir[i] != 0) {
				xloc = currentZoneData.trainerx[i] * 16;
				yloc = currentZoneData.trainery[i] * 16;
				gfx_TransparentSprite(enemySprites[currentZoneData.trainerdir[i] - 1], xloc - 8, yloc + 4);
			}
		}
		else {
			if (currentZoneData.trainerdir[i] != 0) {
				xloc = currentZoneData.trainerx[i] * 16 - screenX;
				yloc = currentZoneData.trainery[i] * 16 - screenY;
				if (xloc < 336 && yloc < 240 && xloc >= 0 && yloc >= 0) {
					gfx_TransparentSprite(enemySprites[currentZoneData.trainerdir[i] - 1], xloc - 8, yloc + 4);
				}
			}
		}
		
	}
}

void map_DrawInformationBar(void) {
	int pokemonIndex;
	gfx_SetColor(colors[0]);
	gfx_FillRectangle(0, 0, 320, 16);
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {
		gfx_TransparentSprite_NoClip(pokeballSprites[pokeballdata[pokemonIndex]], 16 * pokemonIndex, 0);
	}
	sprintf(str, "%u$", playerMoney);
	gfx_PrintStringXY(str, 100, 5);
}

void map_LoseFight(void) {
	int tileIndex, partyIndex, moveIndex;
	text_Display("All your pokemon have fainted", false);
	for (partyIndex = 0; partyIndex < 6; partyIndex++) {
		party[partyIndex].currenthealth = stats_CalculateStats(party[partyIndex]).health;
		party[partyIndex].currentstatus = 0;
		for (moveIndex = 0; moveIndex < 4; moveIndex++) {
			party[partyIndex].pp[moveIndex] = data_moves[party[partyIndex].moves[moveIndex]].uses;
		}
	}
	playerX = 5*16;
	playerY = 7*16;
	indoors = false;
	currentZone = 0;
	moveDir = 3;
	tilemap.width = OUTDOORWIDTH;
	tilemap.height = OUTDOORHEIGHT;
	tx = playerX / 16;
	ty = playerY / 16;
	LoadMap();
}

void map_WinFight(bool wild, uint16_t rewardMoney) {
	char str1[16];
	if (!wild) {
		playerMoney += rewardMoney;
		if (indoors) {
			defeatedTrainersIndoors[currentBuilding][currentTrainer] = true;
			if (currentZoneData.trainerreward[currentTrainer] != 0) {
				if (currentZoneData.trainerreward[currentTrainer] == 255) {
					badgeCount++;
					sprintf(str, "Recieved a badge");
				}
				else {
					playerItems[currentZoneData.trainerreward[currentTrainer] - 1]++;
					items_IndexToName(str1, currentZoneData.trainerreward[currentTrainer] - 1);
					if (currentZoneData.trainerreward[currentTrainer] - 1 >= 20) {
						sprintf(str, "Recieved the TM for %s", str1);
					}
					else {
						sprintf(str, "Recieved a %s", str1);
					}
				}
				text_Display(str, false);
			}
		}
		else {
			defeatedTrainers[currentZone][currentTrainer] = true;
			if (currentZoneData.trainerreward[currentTrainer] != 0) {
				playerItems[currentZoneData.trainerreward[currentTrainer]-1]++;
				items_IndexToName(str1, currentZoneData.trainerreward[currentTrainer] - 1);
				if (currentZoneData.trainerreward[currentTrainer] - 1 >= 20) {
					sprintf(str, "Recieved the TM for %s", str1);
				}
				else {
					sprintf(str, "Recieved a %s", str1);
				}
				text_Display(str, false);
			}
		}
	}
}