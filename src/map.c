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
#include "gfx/test_gfx.h"
#include "items.h"

#include "gfx/PKMNSD4.h"
#include "gfx/PKMNSD7.h"
#include "gfx/PKMNSD8.h"

#define OUTDOORWIDTH 48
#define OUTDOORHEIGHT 40
#define MAX_X ((OUTDOORWIDTH - 21)*16)
#define MAX_Y ((OUTDOORHEIGHT - 15)*16)

void HealParty(void);
void OpenBox(void);
void ReadText(uint8_t index);
void EnterDoor(uint8_t index);
void ExitBuilding(void);
void ExitZone(uint8_t index);
bool FightTrainer(uint8_t index);

int32_t Clamp(int32_t number, int min, int max);
/* Gets the tile the player is facing*/
uint8_t GetNextTile(uint8_t tx, uint8_t ty, int width);
/* Gets the typemap at a specific tile */
uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width);
void DrawPlayer(void);
/* Draw 16 npcs */
void ProcessNpcs(void);
/* Loads the tilemap, typemap ,and zonedata for current area */
void LoadMap(void);

void GrassAnimation(bool part);

const int battlechance = 10; /* Chance for enemy to spawn out of 100 */


uint8_t pokeballdata[] = {
	0,0,0,0,0,0
};

int32_t screenX = 0;
int32_t screenY = 0;

uint8_t tx; /* x tile */
uint8_t ty; /* y tile */
bool running = false;
bool currentRun = 0;
int currentWater = 0;

/* Variables for the leaves that appear when walking through grass */
bool grassAnimNext = 0;
int grassAnimState1 = 0;
int grassAnimState2 = 0;
int grassAnimX1 = 0;
int grassAnimY1 = 0;
int grassAnimX2 = 0;
int grassAnimY2 = 0;

uint8_t npcMoveState[16];
uint8_t npcOldTiles[16];
int npcToFight = 16;
uint8_t npcGrassAnimState[16];

uint8_t npcPatterns[8][16] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,4,4,4,2,5,5,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

uint8_t nextTile;
/* Set to 8 when anything is pressed and decreases each frame until 0 */
uint8_t moveState = 0;
/* Direction Player is facing */
uint8_t moveDir = 3;
uint8_t playerState = 0;

gfx_tilemap_t tilemap;

gfx_sprite_t *mapTiles[128];
gfx_sprite_t *pokeballSprites[3];

uint8_t currentTileMap[1920];
uint8_t currentTypeMap[1920];
struct zoneData currentZoneData;

uint8_t currentTrainer = 0; /* 0-15 */
struct pokemonData tempcharacter2;
const struct pokemonData clearcharacter2 = {0};

char str1[20];

void map_Initialize(void) {
	PKMNSD4_init();
	PKMNSD7_init();
	PKMNSD8_init();

	gfx_SetPalette(map_gfx_pal, sizeof_map_gfx_pal, 0);
	SetColors(0);
	gfx_SetTextFGColor(colors[1]);
	textBoxType = 0;	
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
			if (data_tileSets[currentZone] == 0) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset0_tiles_compressed[tileIndex]);
			}
			else {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset1_tiles_compressed[tileIndex]);
			}
		}
	}
	for (tileIndex = 0; tileIndex < 3; tileIndex++) {
		pokeballSprites[tileIndex] = gfx_MallocSprite(16, 16);
		zx7_Decompress(pokeballSprites[tileIndex], pokeball_tiles_compressed[tileIndex]);
	}
	textBoxType = 0;
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
		gfx_sprite_t *pauseMenuSprite;
		Wait(20);
		gfx_Blit(gfx_screen);
		map_End();
		pauseMenuSprite = gfx_MallocSprite(112, 96);
		zx7_Decompress(pauseMenuSprite, pausemenu_compressed);
		gfx_Sprite(pauseMenuSprite, 195, 25);

		menuReturn = menu_Menu();
		free(pauseMenuSprite);
		map_SetupGfx();
		map_LoadPokeballs();
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return menuReturn;
	}
	/* If player presses 2nd */
	if ((kb_Data[1] & kb_2nd)) {
		nextTile = GetNextTile(tx, ty, tilemap.width);
		if (GetNextTile(tx, ty, tilemap.width) >= 0x1A && GetNextTile(tx, ty, tilemap.width) <= 0x20) {
			int partyIndex, moveIndex;
			bool hasUser = false;
			for (partyIndex = 0; partyIndex < 6; partyIndex++) {
				for (moveIndex = 0; moveIndex < 4; moveIndex++) {
					if (party[partyIndex].moves[moveIndex] == 141) {
						hasUser = true;
						sprintf(str, "%s used Surf", data_pokemon[party[partyIndex].id].name);
						partyIndex = 6;
					}
				}
			}
			if (hasUser) {
				text_Display(str,true);
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
			else {
				text_Display("A Pokemon with Surf could bring you across this water", true);
			}
		}
		else if (nextTile == 0x41) {
			HealParty();
		}
		else if (nextTile == 0x42) {
			gfx_sprite_t *shopSpriteOriginal;
			map_End();

			shopSpriteOriginal = gfx_MallocSprite(134, 84);
			zx7_Decompress(shopSpriteOriginal, shop_compressed);

			menu_ItemShop(shopSpriteOriginal);

			free(shopSpriteOriginal);
			map_SetupGfx();
		}
		else if (nextTile == 0x43) {
			OpenBox();
		}
		else if (nextTile == 0x44) {
			if (newGame) {
				EnterDoor(15);
			}
			else {
				EnterDoor(0);
			}
		}
		else if (nextTile == 0x45) {
			if (newGame) {
				uint8_t starter = 2;
				int tilePos = tx + (moveDir == 1) - (moveDir == 2) + ((ty + (moveDir == 3) - (moveDir == 4)) * tilemap.width);

				sprintf(str, "Choose Squirtle as your starter?");
				if (currentTypeMap[tilePos + 1] == 0x45) {
					starter = 0;
					sprintf(str, "Choose Bulbasaur as your starter?");
					if (currentTypeMap[tilePos - 1] == 0x45) {
						starter = 1;
						sprintf(str, "Choose Charmander as your starter?");
					}
				}

				
				text_Display(str, false);
				if (text_AskQuestion2("Yes", "No", false) == 1) {
					party[0] = stats_NewCharacter((1 + (starter) * 3), 5);
					currentTileMap[tilePos]--;
					newGame = false;
					rivalPokemon = (starter + 1) % 3;
				}
			}
			else {
				text_Display("You already got a Pokemon", true);
			}
		}
		else if (nextTile == 0x46) {
			int partyIndex, moveIndex;
			bool hasUser = false;
			int oldTile;
			int tempTile;
			int newTile = 0;
			oldTile = tx + ty * OUTDOORWIDTH;
			if (moveDir == 1) {
				oldTile = (tx + 1) + (ty)* OUTDOORWIDTH;
				newTile = (tx + 2) + (ty)* OUTDOORWIDTH;
			}
			if (moveDir == 2) {
				oldTile = (tx - 1) + (ty)* OUTDOORWIDTH;
				newTile = (tx - 2) + (ty)* OUTDOORWIDTH;
			}
			if (moveDir == 3) {
				oldTile = (tx)+(ty + 1)* OUTDOORWIDTH;
				newTile = (tx)+(ty + 2)* OUTDOORWIDTH;
			}
			if (moveDir == 4) {
				oldTile = (tx)+(ty - 1)* OUTDOORWIDTH;
				newTile = (tx)+(ty - 2)* OUTDOORWIDTH;
			}
			for (partyIndex = 0; partyIndex < 6; partyIndex++) {
				for (moveIndex = 0; moveIndex < 4; moveIndex++) {
					if (party[partyIndex].moves[moveIndex] == 133) {
						hasUser = true;
						sprintf(str, "%s used Strength", data_pokemon[party[partyIndex].id].name);
						partyIndex = 6;
					}
				}
			}
			if (hasUser && currentTypeMap[newTile] == 0) {
				text_Display(str, true);
				tempTile = currentTileMap[oldTile];
				currentTileMap[oldTile] = currentTileMap[newTile];
				currentTileMap[newTile] = tempTile;
				tempTile = currentTypeMap[oldTile];
				currentTypeMap[oldTile] = currentTypeMap[newTile];
				currentTypeMap[newTile] = tempTile;
			}
			else if (currentTypeMap[newTile] == 0) {
				text_Display("This boulder could be moved by a strong Pokemon", true);
			}
		}
		else if (nextTile == 0x47) {
			int partyIndex, moveIndex;
			bool hasUser = false;
			for (partyIndex = 0; partyIndex < 6; partyIndex++) {
				for (moveIndex = 0; moveIndex < 4; moveIndex++) {
					if (party[partyIndex].moves[moveIndex] == 26) {
						hasUser = true;
						sprintf(str, "%s used Cut", data_pokemon[party[partyIndex].id].name);
						partyIndex = 6;
					}
				}
			}
			if (hasUser) {
				text_Display(str, true);
				if (moveDir == 1) {
					currentTypeMap[(tx + 1) + (ty)* OUTDOORWIDTH] = 0;
					currentTileMap[(tx + 1) + (ty)* OUTDOORWIDTH] = 0;
				}
				if (moveDir == 2) {
					currentTypeMap[(tx - 1) + (ty)* OUTDOORWIDTH] = 0;
					currentTileMap[(tx - 1) + (ty)* OUTDOORWIDTH] = 0;
				}
				if (moveDir == 3) {
					currentTypeMap[(tx)+(ty + 1)* OUTDOORWIDTH] = 0;
					currentTileMap[(tx)+(ty + 1)* OUTDOORWIDTH] = 0;
				}
				if (moveDir == 4) {
					currentTypeMap[(tx)+(ty - 1)* OUTDOORWIDTH] = 0;
					currentTileMap[(tx)+(ty - 1)* OUTDOORWIDTH] = 0;
				}
			}
			else {
				text_Display("This tree could be cut down", true);
			}
		}
		else if (nextTile >= 0x50 && nextTile < 0x60) {
			EnterDoor(nextTile - 80);
		}
		else if (nextTile >= 0x60 && nextTile < 0x70) {
			if (FightTrainer(nextTile - 96)) {
				return 1;
			}
		}
		else if (nextTile >= 0x70 && nextTile < 0x80) {
			ReadText(nextTile - 0x80);
		}
		while ((kb_Data[1] & kb_2nd))
		{
			kb_Scan();
		}
	}
	/* If moving */
	if (moveState > 0) {
		//Wait(1);
		moveState--;
		if (running) {
			Wait(1);
			moveState--;
		}
		if (moveDir == 1) {
			playerX += 2 * (1 + running);
		}
		if (moveDir == 2) {
			playerX -= 2 * (1 + running);
		}
		if (moveDir == 3) {
			playerY += 2 * (1 + running);
		}
		if (moveDir == 4) {
			playerY -= 2 * (1 + running);
		}
		if (surfing) {
			if (moveState > 3) {
				playerState = 1;
			}
			else {
				playerState = 0;
			}
		}
		else if (running){
			if (moveState > 3) {
				playerState = 1 + currentRun;
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
			currentRun = !currentRun;
			/* Arrive At Tile*/
			tx = playerX / 16;
			ty = playerY / 16;
			nextTile = GetTypeMapData(tx, ty, tilemap.width);
			if (nextTile >= 0x10 && nextTile < 0x20) {
				/* Grass */
				if ((rand() % 100) < battlechance) {
					uint8_t wildSpawn;
					wildSpawn = rand() % 5;
					battle_SpawnWild(currentZoneData.spawnids[wildSpawn][nextTile -16], currentZoneData.spawnminlevels[wildSpawn][nextTile - 16], currentZoneData.spawnmaxlevels[wildSpawn][nextTile - 16]);
					return 1;
				}
			}
			else if (nextTile == 0x01) {
				ExitBuilding();
			}
			else if (surfing && !(nextTile >= 0x1A && nextTile < 0x20)) {
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
			if (GetNextTile(tx, ty, tilemap.width) < 64 && !(!surfing && GetNextTile(tx, ty, tilemap.width) >= 0x1A && GetNextTile(tx, ty, tilemap.width) < 0x20)) {
				moveState = 8;
			}
		}
		running = ((kb_Data[2] & kb_Alpha) && moveState > 0 && !surfing);
	}

	map_Draw();
	playerState = 0;

	gfx_SwapDraw();

	if (npcToFight != 16) {
		if (FightTrainer(npcToFight)) {
			npcToFight = 16;
			return 1;
		}
		npcToFight = 16;
	}

	return 0;
}
void map_End(void) {
	int tileIndex = 0;
	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		free(mapTiles[tileIndex]);
	}
	for (tileIndex = 0; tileIndex < 3; tileIndex++) {
		free(pokeballSprites[tileIndex]);
	}
	
}

void map_Draw(void) {
	/* Move Screen and Redraw */
	if (indoors) {
		screenX = 0;
		screenY = 0;

		gfx_Tilemap(&tilemap, screenX, screenY);

		DrawPlayer();
	}
	else {
		/* Animate Water Every 20 Frames */
		if (data_tileSets[currentZone] == 0) {
			if (currentWater == 20) {
				memcpy(mapTiles[29], animatedwater1, 258);
			}
			if (currentWater == 0) {
				memcpy(mapTiles[29], animatedwater2, 258);
			}
			currentWater++;
			if (currentWater == 40) {
				currentWater = 0;
			}
		}

		screenX = Clamp(playerX - 160, 0, MAX_X);
		screenY = Clamp(playerY - 112, 0, MAX_Y);

		gfx_Tilemap(&tilemap, screenX, screenY);

		DrawPlayer();
		

	}
	ProcessNpcs();
	map_DrawInformationBar();
}
void DrawPlayer(void) {
	GrassAnimation(0);


	gfx_TransparentSprite_NoClip((gfx_sprite_t*)PKMNSD8[(24 * surfing) + (12 * running) + (moveDir - 1) * (3 - surfing) + playerState], playerX - screenX - 8, playerY - screenY + 4);

	/* Draw overlay if player is in grass */
	if (currentTileMap[tx + (moveDir == 1) - (moveDir == 2) + (ty + (moveDir == 3) - (moveDir == 4)) * tilemap.width] == 2) {
		/* Always draw grass overlay if moving sideways*/
		if (moveDir <= 2 && moveState > 0) {
			gfx_TransparentSprite_NoClip(grassoverlay1, (tx + (moveDir == 1) - (moveDir == 2)) * 16 - screenX - 8, (ty + (moveDir == 3) - (moveDir == 4)) * 16 - screenY + 18);
		}
		/* Only draw the overlay at the end if moving vertically */
		else if (moveState == 1) {
			gfx_TransparentSprite_NoClip(grassoverlay1, (tx + (moveDir == 1) - (moveDir == 2)) * 16 - screenX - 8, (ty + (moveDir == 3) - (moveDir == 4)) * 16 - screenY + 18);
		}
		if (moveState == 1) {
			if (grassAnimNext) {
				grassAnimState1 = 10;
				grassAnimX1 = (tx + (moveDir == 1) - (moveDir == 2)) * 16;
				grassAnimY1 = (ty + (moveDir == 3) - (moveDir == 4)) * 16;
			}
			else {
				grassAnimState2 = 10;
				grassAnimX2 = (tx + (moveDir == 1) - (moveDir == 2)) * 16;
				grassAnimY2 = (ty + (moveDir == 3) - (moveDir == 4)) * 16;
			}
			grassAnimNext = !grassAnimNext;
		}
	}
	if (!(moveDir == 3 && moveState != 0) && currentTileMap[tx + ty * OUTDOORWIDTH] == 2) {
		gfx_TransparentSprite_NoClip(grassoverlay1, tx * 16 - screenX - 8, ty * 16 - screenY + 18);
	}
	GrassAnimation(1);
}
void GrassAnimation(bool part) {
	if (grassAnimState1 > 0) {
		if ((moveDir == 3 && moveState != 0 && ty == grassAnimY1 / 16) != part) {
			if (grassAnimState1 > 5) {
				gfx_TransparentSprite_NoClip(grassoverlay3, grassAnimX1 - screenX - 8, grassAnimY1 - screenY + 13);
			}
			else {
				gfx_TransparentSprite_NoClip(grassoverlay2, grassAnimX1 - screenX - 8, grassAnimY1 - screenY + 14);
			}
		}
		grassAnimState1--;
	}
	if (grassAnimState2 > 0) {
		if ((moveDir == 3 && moveState != 0 && ty == grassAnimY2 / 16) != part) {
			if (grassAnimState2 > 5) {
				gfx_TransparentSprite_NoClip(grassoverlay3, grassAnimX2 - screenX - 8, grassAnimY2 - screenY + 13);
			}
			else {
				gfx_TransparentSprite_NoClip(grassoverlay2, grassAnimX2 - screenX - 8, grassAnimY2 - screenY + 14);
			}
		}
		grassAnimState2--;
	}
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
}
void ReadText(uint8_t index) {
	text_Display(data_text[currentZoneData.textIds[index]], false);
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
	LoadMap();
}
void ExitBuilding(void) {
	int tileIndex;
	playerX = lastPlayerX;
	playerY = lastPlayerY;
	indoors = false;
	if (data_tileSets[currentZone] == 0) {
		for (tileIndex = 0; tileIndex < 128; tileIndex++) {
			zx7_Decompress(mapTiles[tileIndex], outdoortileset0_tiles_compressed[tileIndex]);
		}
	}
	else {
		for (tileIndex = 0; tileIndex < 128; tileIndex++) {
			zx7_Decompress(mapTiles[tileIndex], outdoortileset1_tiles_compressed[tileIndex]);
		}
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
		text_Display(data_text[currentZoneData.trainertext[index]], true);
		battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
		return true;
	}
	else if (!indoors && !defeatedTrainers[currentZone][currentTrainer]) {
		text_Display(data_text[currentZoneData.trainertext[index]], true);
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
	int i, j;
	int npcIndex;
	ti_var_t mapAV;
	if (indoors) {
		mapAV = ti_Open("PKMNMD1", "r");
		ti_Seek(((int)currentBuilding) * 1270, 0, mapAV);
		ti_Read(&currentTileMap, 315, 1, mapAV);
		ti_Read(&currentTypeMap, 315, 1, mapAV);
		ti_Read(&currentZoneData, sizeof(currentZoneData), 1, mapAV);
	}
	else {
		mapAV = ti_Open("PKMNMD0", "r");
		ti_Seek(((int)currentZone) * 4480, 0, mapAV);
		ti_Read(&currentTileMap, 1920, 1, mapAV);
		ti_Read(&currentTypeMap, 1920, 1, mapAV);
		ti_Read(&currentZoneData, sizeof(currentZoneData), 1, mapAV);
	}
	ti_CloseAll();
	for (npcIndex = 0; npcIndex < 16; npcIndex++) {
		npcMoveState[npcIndex] = 0;
		npcGrassAnimState[npcIndex] = 0;
		npcOldTiles[npcIndex] = currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tilemap.width * currentZoneData.trainery[npcIndex])];
		currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tilemap.width * currentZoneData.trainery[npcIndex])] = 96 + npcIndex;
	}
	/*for (i = 0; i < 42; i++) {
		for (j = 0; j < 16; j++) {
			uint8_t test = *(&currentZoneData.spawnids[0][0] + i * 16 + j);
			dbg_sprintf(dbgout, "%u ", test);
		}
		dbg_sprintf(dbgout, "\n\n");
	}*/
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

void ProcessNpcs(void) {
	int npcIndex, tileIndex, xloc, yloc, xlocMod, ylocMod;
	for (npcIndex = 0; npcIndex < 16; npcIndex++) {
		if (currentZoneData.trainerdir[npcIndex] != 0) {
			xloc = currentZoneData.trainerx[npcIndex] * 16 - screenX;
			yloc = currentZoneData.trainery[npcIndex] * 16 - screenY;
			xlocMod = 0;
			ylocMod = 0;

			if (currentZoneData.trainertype[npcIndex] == 2) {

			}

			if (xloc < 336 && yloc < 240 && xloc >= 0 && yloc >= 0) {
				bool canMove;
				int trainerDir = currentZoneData.trainerdir[npcIndex];
				canMove = true;
				for (tileIndex = 1; tileIndex < 5; tileIndex++) {
					if (currentTypeMap[(currentZoneData.trainerx[npcIndex] + (trainerDir == 1)*tileIndex - (trainerDir == 2)*tileIndex) + tilemap.width * (currentZoneData.trainery[npcIndex] + (trainerDir == 3) * tileIndex - (trainerDir == 4) * tileIndex)] >= 64) {
						if (tileIndex == 1 && (npcMoveState[npcIndex] % 16 == 0) && npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4) {
							canMove = false;
						}
						break;
					}
					if ((currentZoneData.trainerx[npcIndex] + (trainerDir == 1)*tileIndex - (trainerDir == 2)*tileIndex) == tx && (currentZoneData.trainery[npcIndex] + (trainerDir == 3) * tileIndex - (trainerDir == 4) * tileIndex) == ty) {
						if (tileIndex == 1 && (npcMoveState[npcIndex] % 16 == 0) && npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4) {
							canMove = false;
						}
						npcToFight = npcIndex;
						break;
					}
				}

				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 1 && npcMoveState[npcIndex] % 16 == 8) {
					if (trainerDir == 1) {
						currentZoneData.trainerdir[npcIndex] = 4;
					}
					if (trainerDir == 2) {
						currentZoneData.trainerdir[npcIndex] = 3;
					}
					if (trainerDir == 3) {
						currentZoneData.trainerdir[npcIndex] = 2;
					}
					if (trainerDir == 4) {
						currentZoneData.trainerdir[npcIndex] = 1;
					}
				}
				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 2 && npcMoveState[npcIndex] % 16 == 8) {
					if (trainerDir == 1 || trainerDir == 3) {
						currentZoneData.trainerdir[npcIndex]++;
					}
					if (trainerDir == 2 || trainerDir == 4) {
						currentZoneData.trainerdir[npcIndex]--;
					}
				}
				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 1 && npcMoveState[npcIndex] % 16 == 8) {
					if (trainerDir == 1) {
						currentZoneData.trainerdir[npcIndex] = 3;
					}
					if (trainerDir == 2) {
						currentZoneData.trainerdir[npcIndex] = 4;
					}
					if (trainerDir == 3) {
						currentZoneData.trainerdir[npcIndex] = 1;
					}
					if (trainerDir == 4) {
						currentZoneData.trainerdir[npcIndex] = 2;
					}
				}

				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove) {
					if (trainerDir == 1) {
						xlocMod += (npcMoveState[npcIndex] % 16);
					}
					if (trainerDir == 2) {
						xlocMod -= (npcMoveState[npcIndex] % 16);
					}
					if (trainerDir == 3) {
						ylocMod += (npcMoveState[npcIndex] % 16);
					}
					if (trainerDir == 4) {
						ylocMod -= (npcMoveState[npcIndex] % 16);
					}
					if (npcMoveState[npcIndex] % 16 == 0) {
						currentTypeMap[(currentZoneData.trainerx[npcIndex] + ((trainerDir == 1) - (trainerDir == 2))) + (tilemap.width * (currentZoneData.trainery[npcIndex] + ((trainerDir == 3) - (trainerDir == 4))))] = 96 + npcIndex;
					}
					if (npcMoveState[npcIndex] % 16 == 15) {
						currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tilemap.width * currentZoneData.trainery[npcIndex])] = 0;
						currentZoneData.trainerx[npcIndex] += ((trainerDir == 1) - (trainerDir == 2));
						currentZoneData.trainery[npcIndex] += ((trainerDir == 3) - (trainerDir == 4));
					}
				}


				
				gfx_TransparentSprite((gfx_sprite_t*)PKMNSD7[(currentZoneData.trainerskin[npcIndex] * 12) + ((trainerDir-1) * 3) + ((npcMoveState[npcIndex] % 16) / 4 == 1 && (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove)) + 2 * ((npcMoveState[npcIndex] % 16) / 4 == 3 && (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove))], xloc + xlocMod - 8, yloc + ylocMod + 3);

				/* Grass */
				if (currentTileMap[currentZoneData.trainerx[npcIndex] + tilemap.width * currentZoneData.trainery[npcIndex]] == 2 && !indoors) {
					if (trainerDir != 3 || npcMoveState[npcIndex] % 16 <= 1 || !canMove || npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] != 4) {
						if (npcGrassAnimState[npcIndex] > 5 && npcMoveState[npcIndex] % 16 < 14) {
							gfx_TransparentSprite(grassoverlay3, xloc - 8, yloc + 13);
						}
						else if (npcGrassAnimState[npcIndex] > 0 && npcMoveState[npcIndex] % 16 < 14) {
							gfx_TransparentSprite(grassoverlay2, xloc - 8, yloc + 14);
						}
						else {
							gfx_TransparentSprite(grassoverlay1, xloc - 8, yloc + 18);
						}
					}
					if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove && (trainerDir != 4 || npcMoveState[npcIndex] % 16 >= 14)) {
						if (npcMoveState[npcIndex] % 16 == 14) {
							npcGrassAnimState[npcIndex] = 10;
						}
						if (npcGrassAnimState[npcIndex] > 5 && npcMoveState[npcIndex] % 16 >= 14) {
							gfx_TransparentSprite(grassoverlay3, xloc - 8 + 16 * ((trainerDir == 1) - (trainerDir == 2)), yloc + 13 + 16 * ((trainerDir == 3) - (trainerDir == 4)));
						}
						else if (npcGrassAnimState[npcIndex] > 0 && npcMoveState[npcIndex] % 16 >= 14) {
							gfx_TransparentSprite(grassoverlay2, xloc - 8 + 16 * ((trainerDir == 1) - (trainerDir == 2)), yloc + 14 + 16 * ((trainerDir == 3) - (trainerDir == 4)));
						}
						else {
							gfx_TransparentSprite(grassoverlay1, xloc - 8 + 16 * ((trainerDir == 1) - (trainerDir == 2)), yloc + 18 + 16 * ((trainerDir == 3) - (trainerDir == 4)));
						}
					}
					if (npcGrassAnimState[npcIndex] > 0) {
						npcGrassAnimState[npcIndex]--;
					}

					if (canMove) {
						npcMoveState[npcIndex]++;
					}


					if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 5) {
						npcMoveState[npcIndex] = 0;
					}
				}

				if (ty - 1 == currentZoneData.trainery[npcIndex]) {
					DrawPlayer();
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