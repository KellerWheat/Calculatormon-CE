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
#include "gfx/PKMNSD9.h"

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
void AddItem(uint8_t index);
bool FightTrainer(uint8_t index);
int32_t Clamp(int32_t number, int min, int max);
uint8_t GetNextTile(uint8_t tx, uint8_t ty, int width);
uint16_t ShiftTile(uint16_t value, uint8_t direction, uint8_t amount);
uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width);
void DrawPlayer(void);
void ProcessNpcs(void);
void LoadMap(void);
void LoadTileset(bool freeOld);
void GrassAnimation(bool part);



/* Chance for enemy to spawn out of 100 */
const int battlechance = 6; 
uint8_t pokeballdata[] = {
	0,0,0,0,0,0
};

int32_t screenX = 0;
int32_t screenY = 0;

uint8_t tx; /* x tile */
uint8_t ty; /* y tile */
bool running = false;
bool currentStep = 0;
bool currentRun = 24;
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
uint8_t showExclamationPoint = 0;
int exclamationX = 0;
int exclamationY = 0;
uint8_t npcGrassAnimState[16];
bool npcSaidMessage[16];

/*0 = wait, 1 = rotate counterclockwise, 2 = turn around, 3 = rotate clockwise, 4 = walk, 5 = repeat*/
uint8_t npcPatterns[8][16] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},// 0 - Stand
	{0,4,4,4,2,5,0,0,0,0,0,0,0,0,0,0},// 1 - Pace back and forth
	{0,4,4,4,3,5,0,0,0,0,0,0,0,0,0,0},// 2 - Circle
	{0,3,0,0,0,1,0,0,5,0,0,0,0,0,0,0},// 3 - Look clockwise then turn back
	{4,4,4,4,4,4,4,4,4,4,4,4,4,4,2,5},// 4 - Long Pace
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
uint8_t jumpState = 0;
uint8_t jumpHeights[17] = {1,3,6,9,12,14,15,16,16,15,14,12,9,6,3,1, 0 };

gfx_tilemap_t tilemap;
uint8_t tileMapWidth = OUTDOORWIDTH;
uint8_t tileMapHeight = OUTDOORHEIGHT;

gfx_sprite_t *mapTiles[128+9];
gfx_sprite_t *pokeballSprites[3];
gfx_sprite_t *animatedWaters[9][8];

/* What map looks like */
uint8_t currentTileMap[1920];
/* How map behaves */
uint8_t currentTypeMap[1920];

struct zoneData currentZoneData;
/* What trainer the player is fighting*/
uint8_t currentTrainer = 0; /* 0-15 */

void map_Initialize(void) {
	int waterIndex1, waterIndex2;
	PKMNSD4_init();
	PKMNSD7_init();
	PKMNSD8_init();
	PKMNSD9_init();

	animatedWaters[0][1] = animatedwater0;
	animatedWaters[0][2] = animatedwater1;
	animatedWaters[0][3] = animatedwater2;
	animatedWaters[0][4] = animatedwater3;
	animatedWaters[0][5] = animatedwater4;
	animatedWaters[0][6] = animatedwater5;
	animatedWaters[0][7] = animatedwater6;
	animatedWaters[0][0] = animatedwater7;
	for (waterIndex1 = 1; waterIndex1 < 9; waterIndex1++) {
		for (waterIndex2 = 0; waterIndex2 < 8; waterIndex2++) {
			animatedWaters[waterIndex1][waterIndex2] = (gfx_sprite_t*)PKMNSD9[(waterIndex1 - 1) * 8 + waterIndex2];
		}
		mapTiles[128 + waterIndex1] = animatedWaters[waterIndex1][0];
	}
	mapTiles[128] = animatedwater0;

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

	if (currentSave.indoors) {
		tilemap.width = 21;
		tilemap.height = 15;
		tileMapWidth = 21;
		tileMapHeight = 15;
	}

	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadMap();
	
}
void map_Setup(void) {
	map_SetupGfx();

	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
}
void map_SetupGfx(void) {
	int tileIndex = 0;
	LoadTileset(false);
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
		bool awaitSecond = true;
		nextTile = GetNextTile(tx, ty, tileMapWidth);
		if (GetNextTile(tx, ty, tileMapWidth) >= 0x1A && GetNextTile(tx, ty, tileMapWidth) < 0x20 && !currentSave.surfing) {
			int partyIndex, moveIndex;
			bool hasUser = false;
			for (partyIndex = 0; partyIndex < 6; partyIndex++) {
				for (moveIndex = 0; moveIndex < 4; moveIndex++) {
					if (currentSave.party[partyIndex].moves[moveIndex] == 141) {
						hasUser = true;
						sprintf(str, "%s used Surf", data_pokemon[currentSave.party[partyIndex].id].name);
						partyIndex = 6;
					}
				}
			}
			if (hasUser) {
				text_Display(str);
				if (moveDir == 1) {
					currentSave.playerX += 16;
				}
				if (moveDir == 2) {
					currentSave.playerX -= 16;
				}
				if (moveDir == 3) {
					currentSave.playerY += 16;
				}
				if (moveDir == 4) {
					currentSave.playerY -= 16;
				}
				currentSave.surfing = true;
				tx = currentSave.playerX / 16;
				ty = currentSave.playerY / 16;
			}
			else {
				text_Display("A Pokemon with Surf could bring you\nacross this water");
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
			if (currentSave.newGame) {
				EnterDoor(15);
			}
			else {
				EnterDoor(0);
			}
		}
		else if (nextTile == 0x45) {
			if (currentSave.newGame) {
				uint8_t starter = 2;
				int tilePos = tx + (moveDir == 1) - (moveDir == 2) + ((ty + (moveDir == 3) - (moveDir == 4)) * tileMapWidth);

				sprintf(str, "Choose Squirtle as your starter?");
				if (currentTypeMap[tilePos + 1] == 0x45) {
					starter = 0;
					sprintf(str, "Choose Bulbasaur as your starter?");
					if (currentTypeMap[tilePos - 1] == 0x45) {
						starter = 1;
						sprintf(str, "Choose Charmander as your starter?");
					}
				}

				
				text_Display(str);
				if (text_AskQuestion2("Yes", "No") == 1) {
					currentSave.party[0] = stats_NewCharacter((1 + (starter) * 3), 5);
					currentTileMap[tilePos]-=3;
					currentSave.newGame = false;
					currentSave.worldState++;
					currentSave.rivalPokemon = (starter + 1) % 3;
				}
				map_LoadPokeballs();
			}
			else {
				text_Display("You already chose a Pokemon");
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
					if (currentSave.party[partyIndex].moves[moveIndex] == 133) {
						hasUser = true;
						sprintf(str, "%s used Strength", data_pokemon[currentSave.party[partyIndex].id].name);
						partyIndex = 6;
					}
				}
			}
			if (hasUser && currentTypeMap[newTile] == 0) {
				text_Display(str);
				tempTile = currentTileMap[oldTile];
				currentTileMap[oldTile] = currentTileMap[newTile];
				currentTileMap[newTile] = tempTile;
				tempTile = currentTypeMap[oldTile];
				currentTypeMap[oldTile] = currentTypeMap[newTile];
				currentTypeMap[newTile] = tempTile;
			}
			else if (currentTypeMap[newTile] == 0) {
				text_Display("This boulder could be moved by a\nstrong Pokemon");
			}
		}
		else if (nextTile == 0x47) {
			int partyIndex, moveIndex;
			bool hasUser = false;
			for (partyIndex = 0; partyIndex < 6; partyIndex++) {
				for (moveIndex = 0; moveIndex < 4; moveIndex++) {
					if (currentSave.party[partyIndex].moves[moveIndex] == 26) {
						hasUser = true;
						sprintf(str, "%s used Cut", data_pokemon[currentSave.party[partyIndex].id].name);
						partyIndex = 6;
					}
				}
			}
			if (hasUser) {
				text_Display(str);
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
				text_Display("This tree could be cut down");
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
			ReadText(nextTile - 0x70);
		}
		else if (nextTile >= 0x80 && nextTile < 0x90) {
			if (!currentSave.takenGroundGifts[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][nextTile - 0x80]) {
				char str1[20];
				if (currentSave.playerItems[currentZoneData.grounditems[nextTile - 0x80] - 1] == 255) {
					items_IndexToName(str1, currentZoneData.grounditems[nextTile - 0x80] - 1);
					sprintf(str, "You cannot hold this %s", str1);
					text_Display(str);
				}
				else {
					currentSave.takenGroundGifts[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][nextTile - 0x80] = true;
					currentSave.playerItems[currentZoneData.grounditems[nextTile - 0x80] - 1]++;
					items_IndexToName(str1, currentZoneData.grounditems[nextTile - 0x80] - 1);
					if (currentZoneData.grounditems[nextTile - 0x80] - 1 >= 20) {
						sprintf(str, "Found the TM for %s", str1);
					}
					else {
						sprintf(str, "Found a %s", str1);
					}
					text_Display(str);
					currentTileMap[ShiftTile(tx + tileMapWidth *ty, moveDir, 1)] = 0;
					currentTypeMap[ShiftTile(tx + tileMapWidth *ty, moveDir, 1)] = 0;
					currentSave.foundItemsPos[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][nextTile - 0x80] = ShiftTile(tx + tileMapWidth *ty, moveDir, 1);
				}
			}
		}
		else {
			awaitSecond = false;
		}
		if (awaitSecond) {
			while ((kb_Data[1] & kb_2nd))
			{
				kb_Scan();
			}
		}
	}
	/* If moving */
	if (moveState > 0) {
		moveState--;
		if (jumpState) {
			jumpState--;
		}
		if (running) {
			moveState--;
			if (jumpState) {
				jumpState--;
			}
			Wait(1);
		}
		if (moveDir == 1) {
			currentSave.playerX += 2 * (1 + running);
		}
		if (moveDir == 2) {
			currentSave.playerX -= 2 * (1 + running);
		}
		if (moveDir == 3) {
			currentSave.playerY += 2 * (1 + running);
		}
		if (moveDir == 4) {
			currentSave.playerY -= 2 * (1 + running);
		}
		if (currentSave.surfing) {
			if (moveState > 3) {
				playerState = 1;
			}
			else {
				playerState = 0;
			}
		}
		else if (running){
			if (moveState > 3) {
				playerState = 1 + currentStep;
			}
			else {
				playerState = 0;
			}
		}
		else {
			if (moveState > 3) {
				playerState = 1 + currentStep;
			}
			else if (moveState >= 0) {
				playerState = 0;
			}
		}

		if (moveState == 0) {
			if (currentStep) {
				currentRun = !currentRun;
			}
			currentStep = !currentStep;
			playerState = 0;
			/* Arrive At Tile*/
			tx = currentSave.playerX / 16;
			ty = currentSave.playerY / 16;
			nextTile = GetTypeMapData(tx, ty, tileMapWidth);
			if (nextTile >= 0x10 && nextTile < 0x20) {
				/* Grass */
				if ((rand() % 100) < battlechance) {
					uint8_t wildSpawn;
					map_Draw();
					gfx_SwapDraw();
					wildSpawn = rand() % 5;
					battle_SpawnWild(currentZoneData.spawnids[wildSpawn][nextTile -16], currentZoneData.spawnminlevels[wildSpawn][nextTile - 16], currentZoneData.spawnmaxlevels[wildSpawn][nextTile - 16]);
					return 1;
				}
			}
			else if (nextTile != 0) {
				running = false;
				map_Draw();
				gfx_SwapDraw();
				if (currentSave.surfing && !(nextTile >= 0x1A && nextTile < 0x20)) {
					currentSave.surfing = false;
				}
				if (nextTile == 0x01) {
					ExitBuilding();
				}
				else if (nextTile == 0x02) {
					if (((currentSave.indoors && !currentSave.defeatedTrainersIndoors[currentSave.currentZone][15]) || (!currentSave.indoors && !currentSave.defeatedTrainers[currentSave.currentZone][15])) && currentZoneData.trainerdir[15] != 0) {
						if (currentZoneData.trainerx[15] == 255) {
							int walkState, xloc, yloc;

							if (currentZoneData.trainerdir[15] <= 2) {
								if (currentZoneData.trainerdir[15] == 1) {
									xloc = -8;
									yloc = currentSave.playerY - screenY + 3;
									walkState = currentSave.playerX - screenX - 16;
								}
								else {
									xloc = 312;
									yloc = currentSave.playerY - screenY + 3;
									walkState = 320 - (currentSave.playerX - screenX + 16);
								}
								
							}
							else {
								if (currentZoneData.trainerdir[15] == 3) {
									xloc = currentSave.playerX - screenX - 8;
									yloc = 3;
									walkState = currentSave.playerY - screenY - 16;
								}
								else {
									xloc = currentSave.playerX - screenX - 8;
									yloc = 243;
									walkState = 240-(currentSave.playerY - screenY + 16);
								}
								
							}
							while (walkState > 0)
							{
								map_Draw();
								xloc += ((currentZoneData.trainerdir[15] == 1) - (currentZoneData.trainerdir[15] == 2));
								yloc += ((currentZoneData.trainerdir[15] == 3) - (currentZoneData.trainerdir[15] == 4));

								gfx_TransparentSprite((gfx_sprite_t*)PKMNSD7[(currentZoneData.trainerskin[15] * 12) + ((currentZoneData.trainerdir[15] - 1) * 3) + ((walkState % 16) / 4 == 1) + 2 * ((walkState % 16) / 4 == 3)], xloc, yloc);

								gfx_SwapDraw();

								walkState--;
							}
							if (currentZoneData.trainerdir[15] == 3) {
								gfx_TransparentSprite((gfx_sprite_t*)PKMNSD7[(currentZoneData.trainerskin[15] * 12) + ((currentZoneData.trainerdir[15] - 1) * 3)], xloc, yloc);
								DrawPlayer();
								gfx_SwapDraw();
							}
							currentZoneData.trainerx[15] = 255;
							currentZoneData.trainery[15] = 255;
						}
						FightTrainer(15);
						currentSave.defeatedTrainers[currentSave.currentZone][15] = true;
						return 1;
					}
				}
				else if (nextTile >= 0x20 && nextTile < 0x30) {
					ExitZone(nextTile - 32);
				}
				else if (nextTile >= 0x30 && nextTile < 0x40) {
					if (FightTrainer(nextTile - 48)) {
						return 1;
					}
				}
				else if (nextTile == 0x48) {
					moveState = 8;
				}
			}
		}
	}

	/* If not moving */
	else {
		if (debugging) {
			if (kb_Data[6] & kb_Add) {
				if (currentSave.indoors) {
					currentSave.currentBuilding++;
				}
				else {
					currentSave.currentZone++;
				}
				LoadTileset(true);
				LoadMap();
			}
			if (kb_Data[6] & kb_Sub) {
				if (currentSave.indoors) {
					currentSave.currentBuilding--;
				}
				else {
					currentSave.currentZone--;
				}
				LoadTileset(true);
				LoadMap();
			}
		}
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
			if ((kb_Data[6] & kb_Mul) && debugging) {
				currentSave.playerX += 16 * (moveDir == 1);
				currentSave.playerX -= 16 * (moveDir == 2);
				currentSave.playerY += 16 * (moveDir == 3);
				currentSave.playerY -= 16 * (moveDir == 4);
				tx = currentSave.playerX / 16;
				ty = currentSave.playerY / 16;
			}
			else {
				if (GetNextTile(tx, ty, tileMapWidth) < 64 && !(!currentSave.surfing && GetNextTile(tx, ty, tileMapWidth) >= 0x1A && GetNextTile(tx, ty, tileMapWidth) < 0x20) && !((moveDir == 2 && tx == 0 ) || (moveDir == 1 && tx == tileMapWidth-1) || (moveDir == 4 && ty == 0) || (moveDir == 3 && ty == tileMapHeight-1))) {

					moveState = 8;
				}
				else if (GetNextTile(tx, ty, tileMapWidth) == 0x48 && moveDir == 3) {
					moveState = 8;
					jumpState = 16;
				}
			}
		}
		running = ((kb_Data[2] & kb_Alpha) && moveState > 0 && !currentSave.surfing);
	}

	map_Draw();
	playerState = 0;
	if (showExclamationPoint == 1) {
		gfx_TransparentSprite(exclamationpoint, exclamationX, exclamationY);
	}

	gfx_SwapDraw();

	if (showExclamationPoint == 1) {
		showExclamationPoint = 0;
		Wait(20);
	}

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
	if (currentSave.indoors) {
		screenX = 0;
		screenY = 0;

		gfx_Tilemap(&tilemap, screenX, screenY);

		DrawPlayer();
	}
	else {
		/* Animate Water Every 20 Frames (NOT WORKING) */
		if ((currentWater % 10) == 0) {
			uint8_t waterIndex;
			
			for (waterIndex = 0; waterIndex < 9; waterIndex++) {
				mapTiles[128+waterIndex] = animatedWaters[waterIndex][currentWater / 10];
			}
		}
		currentWater++;
		currentWater++;
		if (currentWater == 80) {
			currentWater = 0;
		}

		screenX = Clamp(currentSave.playerX - 160, 0, MAX_X);
		screenY = Clamp(currentSave.playerY - 112, 0, MAX_Y);

		gfx_Tilemap(&tilemap, screenX, screenY);

		DrawPlayer();


	}
	ProcessNpcs();
	map_DrawInformationBar();
}
void DrawPlayer(void) {
	GrassAnimation(0);

	gfx_TransparentSprite((gfx_sprite_t*)PKMNSD8[(24 * currentSave.surfing) + (12 * running) + (moveDir - 1) * (3 - currentSave.surfing) + playerState], currentSave.playerX - screenX - 8, currentSave.playerY - screenY + 4 - jumpHeights[16-jumpState]);

	/* Draw overlay if player is in grass */
	if (currentTileMap[tx + (moveDir == 1) - (moveDir == 2) + (ty + (moveDir == 3) - (moveDir == 4)) * tileMapWidth] == 2 && !currentSave.indoors) {
		/* Always draw grass overlay if moving sideways*/
		if (moveDir <= 2 && moveState > 0) {
			gfx_TransparentSprite_NoClip(grassoverlay1, (tx + (moveDir == 1) - (moveDir == 2)) * 16 - screenX - 8, (ty + (moveDir == 3) - (moveDir == 4)) * 16 - screenY + 18);
		}
		if ((moveDir == 1 && moveState == 1 + running) || (moveDir == 2 && moveState == 1 + running) || (moveDir == 3 && moveState == 3 + running) || (moveDir == 4 && moveState == 1 + running)) {
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
	if (!(moveDir == 3 && moveState != 0) && currentTileMap[tx + ty * tileMapWidth] == 2 && !currentSave.indoors) {
		gfx_TransparentSprite_NoClip(grassoverlay1, tx * 16 - screenX - 8, ty * 16 - screenY + 18);
	}

	GrassAnimation(1);
}
void GrassAnimation(bool part) {
	if (grassAnimState1 > 0) {
		if (part != ((moveDir == 3 && (currentSave.playerY + 16 > grassAnimY1 + 16) && (currentSave.playerY - 4 < grassAnimY1 + 10)) || (moveDir == 4 && ty != (grassAnimY1 / 16)))) {
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
		if (part != ((moveDir == 3 && (currentSave.playerY + 16 > grassAnimY2 + 16) && (currentSave.playerY - 4 < grassAnimY2 + 10)) || (moveDir == 4 && ty != (grassAnimY2 / 16)))) {
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
		currentSave.party[pokemonIndex].currenthealth = stats_CalculateStats(currentSave.party[pokemonIndex]).health;
		currentSave.party[pokemonIndex].currentstatus = 0;
		for (moveIndex = 0; moveIndex < 4; moveIndex++) {
			currentSave.party[pokemonIndex].pp[moveIndex] = data_moves[currentSave.party[pokemonIndex].moves[moveIndex]].uses;
		}
	}
	text_Display("Your party has been healed");
	map_LoadPokeballs();
}
void OpenBox(void) {
	map_End();
	
	menu_Box();

	map_SetupGfx();
	map_LoadPokeballs();
}
void ReadText(uint8_t index) {
	LoadText(currentZoneData.textIds[index]);
	text_Display(loadedText);
}
void EnterDoor(uint8_t index) {
	int tileIndex;
	currentSave.lastPlayerX = currentSave.playerX;
	currentSave.lastPlayerY = currentSave.playerY;
	currentSave.indoors = true;
	currentSave.currentBuilding = currentZoneData.doorzones[index];
	LoadTileset(true);
	tilemap.width = 21;
	tilemap.height = 15;
	tileMapWidth = 21;
	tileMapHeight = 15;
	currentSave.playerX = currentZoneData.doorx[index] * 16;
	currentSave.playerY = currentZoneData.doory[index] * 16;
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadMap();
}
void ExitBuilding(void) {
	int tileIndex;
	currentSave.playerX = currentSave.lastPlayerX;
	currentSave.playerY = currentSave.lastPlayerY;
	currentSave.indoors = false;
	tilemap.width = OUTDOORWIDTH;
	tilemap.height = OUTDOORHEIGHT;
	tileMapWidth = OUTDOORWIDTH;
	tileMapHeight = OUTDOORHEIGHT;
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadTileset(true);
	LoadMap();
}
void ExitZone(uint8_t index) {
	if (currentZoneData.exitx[index] != 255) {
		currentSave.playerX = currentZoneData.exitx[index] * 16;
	}
	if (currentZoneData.exity[index] != 255) {
		currentSave.playerY = currentZoneData.exity[index] * 16;
	}
	currentSave.currentZone = currentZoneData.exitzone[index];
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadTileset(true);
	LoadMap();
}

void AddItem(uint8_t index) {
	char str1[20];
	if (index != 0) {
		if (index == 255) {
			currentSave.badgeCount++;
			currentSave.worldState++;
			sprintf(str, "You recieved a badge");
		}
		else if (index == 254) {
			currentSave.worldState++;
		}
		else {
			if (currentSave.playerItems[index - 1] != 255) {
				currentSave.playerItems[index - 1]++;
				items_IndexToName(str1, index - 1);
				if (index - 1 >= 20) {
					sprintf(str, "You recieved the TM for\n%s", str1);
				}
				else {
					sprintf(str, "You recieved a %s", str1);
				}
			}
			if (index != 254) {
				text_Display(str);
			}
		}
	}
}	
bool FightTrainer(uint8_t index) {
	if (currentZoneData.trainertype[index] <=1) {
		currentTrainer = index;
		if (currentZoneData.trainerspawnids[0][index] >= 253) {
			currentZoneData.trainerspawnids[0][index] = currentZoneData.trainerspawnids[0][index] - 252 + 3*currentSave.rivalPokemon;
		}
		if (currentSave.indoors && !currentSave.defeatedTrainersIndoors[currentSave.currentBuilding][currentTrainer]) {
			lastTextIndex = 0;
			lastTextID = currentZoneData.trainertext[index];
			LoadText(currentZoneData.trainertext[index]);
			text_Display(loadedText);
			battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
			return true;
		}
		else if (!currentSave.indoors && !currentSave.defeatedTrainers[currentSave.currentZone][currentTrainer]) {
			lastTextIndex = 0;
			lastTextID = currentZoneData.trainertext[index];
			LoadText(currentZoneData.trainertext[index]);
			text_Display(loadedText);
			battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
			return true;
		}
		
	}
	else if (currentZoneData.trainertype[index] <= 3) {
		LoadText(currentZoneData.trainertext[index]);
		text_Display(loadedText);
		if (currentSave.indoors && !currentSave.takenGiftsIndoors[currentSave.currentBuilding][index]) {
			AddItem(currentZoneData.trainerreward[index]);
			currentSave.takenGiftsIndoors[currentSave.currentBuilding][index] = true;
		}
		if (!currentSave.indoors && !currentSave.takenGifts[currentSave.currentZone][index]) {
			AddItem(currentZoneData.trainerreward[index]);
			currentSave.takenGifts[currentSave.currentZone][index] = true;
		}
	}
	else if (currentZoneData.trainertype[index] == 4 && moveState == 0) {
		LoadText(currentZoneData.trainertext[index]);
		text_Display(loadedText);
		moveDir = moveDir + 1 - 2 * !(moveDir % 2);
		moveState = 8;

	}
	else if (currentZoneData.trainertype[index] == 5 && moveState == 0) {
		LoadText(currentZoneData.trainertext[index]);
		text_Display(loadedText);
		moveDir = moveDir + 1 - 2 * !(moveDir % 2);
		moveState = 8;
	}
	else if (currentZoneData.trainertype[index] == 6) {
		if (currentSave.newGame) {
			text_Display("Professor Spruce was looking for\nyou. He is in his Laboratory.");
		}
		else {
			if (!currentSave.takenGiftsIndoors[currentSave.currentBuilding][index]) {
				text_Display("You can use these to catch\nmore Pokemon.\n\nRecieved 5 pokeballs.");
				currentSave.playerItems[0] += 5;
				currentSave.takenGiftsIndoors[currentSave.currentBuilding][index] = true;
			}
			else {
				text_Display("Let me heal your Pokemon.");
				HealParty();
			}
		}
	}
	return false;
}
void map_LoadPokeballs(void) {
	int pokemonIndex;
	/* Generate pokemon info To Display At Top of Screen */
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {
		if (currentSave.party[pokemonIndex].id == 0) {
			pokeballdata[pokemonIndex] = 2;
		}
		else {
			if (currentSave.party[pokemonIndex].currenthealth > 0) {
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
	if (currentSave.indoors) {
		mapAV = ti_Open("PKMNMD1", "r");
		ti_Seek(((int)currentSave.currentBuilding) * (630 + sizeof(currentZoneData)), 0, mapAV);
		ti_Read(&currentTileMap, 315, 1, mapAV);
		ti_Read(&currentTypeMap, 315, 1, mapAV);
		ti_Read(&currentZoneData, sizeof(currentZoneData), 1, mapAV);
	}
	else {
		mapAV = ti_Open("PKMNMD0", "r");
		ti_Seek(((int)currentSave.currentZone) * (3840 + sizeof(currentZoneData)), 0, mapAV);
		ti_Read(&currentTileMap, 1920, 1, mapAV);
		ti_Read(&currentTypeMap, 1920, 1, mapAV);
		ti_Read(&currentZoneData, sizeof(currentZoneData), 1, mapAV);
	}
	ti_CloseAll();
	for (npcIndex = 0; npcIndex < 16; npcIndex++) {
		if (currentZoneData.minworldstate[npcIndex] > 0 && currentZoneData.maxworldstate[npcIndex] > 0) {
			if (currentSave.worldState < currentZoneData.minworldstate[npcIndex] || currentSave.worldState > currentZoneData.maxworldstate[npcIndex]) {
				currentZoneData.trainerdir[npcIndex] = 0;
			}
		}


		npcMoveState[npcIndex] = 0;
		npcGrassAnimState[npcIndex] = 0;
		npcOldTiles[npcIndex] = currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tileMapWidth * currentZoneData.trainery[npcIndex])];
		if (currentZoneData.trainerdir[npcIndex] != 0) {
			currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tileMapWidth * currentZoneData.trainery[npcIndex])] = 96 + npcIndex;
		}
		npcSaidMessage[npcIndex] = false;

		if (currentSave.takenGroundGifts[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][npcIndex]) {
			currentTileMap[currentSave.foundItemsPos[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][npcIndex]] = 0;
			currentTypeMap[currentSave.foundItemsPos[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][npcIndex]] = 0;
		}
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
uint16_t ShiftTile(uint16_t value, uint8_t direction, uint8_t amount) {
	return (value + ((direction == 1) * amount) - ((direction == 2) * amount) + ((direction == 3) * tileMapWidth * amount) - ((direction == 4) * tileMapWidth * amount));
}
uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width) {
	if (currentSave.indoors) {
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

			if (xloc < 352 && yloc < 256 && xloc >= -16 && yloc >= -16) {
				bool canMove;
				int trainerDir = currentZoneData.trainerdir[npcIndex];
				canMove = true;
				for (tileIndex = 1; tileIndex < 5; tileIndex++) {
					/* Checks for walls*/
					if (currentTypeMap[(currentZoneData.trainerx[npcIndex] + (trainerDir == 1)*tileIndex - (trainerDir == 2)*tileIndex) + tileMapWidth * (currentZoneData.trainery[npcIndex] + (trainerDir == 3) * tileIndex - (trainerDir == 4) * tileIndex)] >= 64) {
						if (tileIndex == 1 && (npcMoveState[npcIndex] % 16 == 0) && npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4) {
							canMove = false;
						}
						break;
					}
					/* Checks for player */
					if (((currentZoneData.trainerx[npcIndex] + (trainerDir == 1)*tileIndex - (trainerDir == 2)*tileIndex) == tx && (currentZoneData.trainery[npcIndex] + (trainerDir == 3) * tileIndex - (trainerDir == 4) * tileIndex) == ty)) {
						if (tileIndex == 1 && (npcMoveState[npcIndex] % 16 == 0) && npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4) {
							canMove = false;
						}
						if (currentZoneData.trainertype[npcIndex] == 0 || (currentZoneData.trainertype[npcIndex] == 2 && !npcSaidMessage[npcIndex]) || currentZoneData.trainertype[npcIndex] == 4 || (currentZoneData.trainertype[npcIndex] == 6 && !npcSaidMessage[npcIndex] && currentSave.newGame)) {
							npcSaidMessage[npcIndex] = true;
							npcToFight = npcIndex;
							if (((currentSave.indoors && !currentSave.defeatedTrainersIndoors[currentSave.currentBuilding][npcIndex]) || (!currentSave.indoors && !currentSave.defeatedTrainers[currentSave.currentZone][npcIndex])) && !(moveState != 0 && currentZoneData.trainertype[npcIndex] == 4)) {
								showExclamationPoint = 2;
							}
						}
						break;
					}
					/* Checks for the space player is about to move into*/
					if (moveState != 0 && (currentZoneData.trainerx[npcIndex] + (trainerDir == 1)*tileIndex - (trainerDir == 2)*tileIndex) == (tx + (moveDir == 1) - (moveDir == 2)) && (currentZoneData.trainery[npcIndex] + (trainerDir == 3) * tileIndex - (trainerDir == 4) * tileIndex) == (ty + (moveDir == 3) - (moveDir == 4))) {
						if (tileIndex == 1 && (npcMoveState[npcIndex] % 16 == 0) && npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4) {
							canMove = false;
						}
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
						currentZoneData.trainerdir[npcIndex] = 1;
					}
					if (trainerDir == 4) {
						currentZoneData.trainerdir[npcIndex] = 2;
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
				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 3 && npcMoveState[npcIndex] % 16 == 8) {
					if (trainerDir == 1) {
						currentZoneData.trainerdir[npcIndex] = 3;
					}
					if (trainerDir == 2) {
						currentZoneData.trainerdir[npcIndex] = 4;
					}
					if (trainerDir == 3) {
						currentZoneData.trainerdir[npcIndex] = 2;
					}
					if (trainerDir == 4) {
						currentZoneData.trainerdir[npcIndex] = 1;
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
						currentTypeMap[(currentZoneData.trainerx[npcIndex] + (trainerDir == 1) - (trainerDir == 2) - (trainerDir != 1)) + (tileMapWidth * (currentZoneData.trainery[npcIndex] + (trainerDir == 3) - (trainerDir == 4)))] = 96 + npcIndex;
					}
					if (npcMoveState[npcIndex] % 16 == 15) {
						currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tileMapWidth * currentZoneData.trainery[npcIndex])] = 0;
						currentZoneData.trainerx[npcIndex] += ((trainerDir == 1) - (trainerDir == 2));
						currentZoneData.trainery[npcIndex] += ((trainerDir == 3) - (trainerDir == 4));
					}
				}


				if (currentZoneData.trainerskin[npcIndex] < 128) {
					gfx_TransparentSprite((gfx_sprite_t*)PKMNSD7[(currentZoneData.trainerskin[npcIndex] * 12) + ((trainerDir - 1) * 3) + ((npcMoveState[npcIndex] % 16) / 4 == 1 && (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove)) + 2 * ((npcMoveState[npcIndex] % 16) / 4 == 3 && (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove))], xloc + xlocMod - 8, yloc + ylocMod + 3);
				}
				else {
					gfx_TransparentSprite((gfx_sprite_t*)PKMNSD8[32 + ((currentZoneData.trainerskin[npcIndex] - 128) * 4) + (trainerDir - 1)], xloc + xlocMod - 8, yloc + ylocMod + 3);
				}
				if (showExclamationPoint == 2) {
					showExclamationPoint = 1;
					exclamationX = xloc + xlocMod - 6;
					exclamationY = yloc + ylocMod - 8;
				}
				
				/* Grass */
				if (currentTileMap[currentZoneData.trainerx[npcIndex] + tileMapWidth * currentZoneData.trainery[npcIndex]] == 2 && !currentSave.indoors) {
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

					
				}

				if (canMove) {
					npcMoveState[npcIndex]++;
				}


				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 5) {
					npcMoveState[npcIndex] = 0;
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
	sprintf(str, "%u$", currentSave.playerMoney);
	gfx_PrintStringXY(str, 100, 5);
}
void map_LoseFight(void) {
	int tileIndex, partyIndex, moveIndex;
	text_Display("All your pokemon have fainted");
	for (partyIndex = 0; partyIndex < 6; partyIndex++) {
		currentSave.party[partyIndex].currenthealth = stats_CalculateStats(currentSave.party[partyIndex]).health;
		currentSave.party[partyIndex].currentstatus = 0;
		for (moveIndex = 0; moveIndex < 4; moveIndex++) {
			currentSave.party[partyIndex].pp[moveIndex] = data_moves[currentSave.party[partyIndex].moves[moveIndex]].uses;
		}
	}
	currentSave.playerX = 16*16;
	currentSave.playerY = 20*16;
	currentSave.indoors = false;
	currentSave.currentZone = 0;
	moveDir = 3;
	tilemap.width = OUTDOORWIDTH;
	tilemap.height = OUTDOORHEIGHT;
	tileMapWidth = OUTDOORWIDTH;
	tileMapHeight = OUTDOORHEIGHT;
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadMap();
}
void map_WinFight(bool wild, uint16_t rewardMoney) {
	char str1[16];
	if (!wild) {
		if (lastTextIndex > 0) {
			LoadText(lastTextID);
			text_Display(loadedText + lastTextIndex );
		}
		currentSave.playerMoney += rewardMoney;
		if (currentSave.indoors) {
			currentSave.defeatedTrainersIndoors[currentSave.currentBuilding][currentTrainer] = true;
			AddItem(currentZoneData.trainerreward[currentTrainer]);
		}
		else {
			currentSave.defeatedTrainers[currentSave.currentZone][currentTrainer] = true;
			AddItem(currentZoneData.trainerreward[currentTrainer]);
		}
	}
}
void LoadTileset(bool freeOld) {
	int tileIndex = 0;
	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		if (freeOld) {
			free(mapTiles[tileIndex]);
		}
		mapTiles[tileIndex] = gfx_MallocSprite(16, 16);
		if (currentSave.indoors) {
			if (data_indoortileSets[currentSave.currentBuilding] == 0) {
				zx7_Decompress(mapTiles[tileIndex], indoortileset0_tiles_compressed[tileIndex]);
			}
			else if (data_indoortileSets[currentSave.currentBuilding] == 1) {
				zx7_Decompress(mapTiles[tileIndex], indoortileset1_tiles_compressed[tileIndex]);
			}
			else if (data_indoortileSets[currentSave.currentBuilding] == 2) {
				zx7_Decompress(mapTiles[tileIndex], indoortileset2_tiles_compressed[tileIndex]);
			}
		}
		else {
		
			if (data_tileSets[currentSave.currentZone] == 0) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset0_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 1) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset1_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 2) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset2_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 3) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset3_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 4) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset4_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 5) {
				//zx7_Decompress(mapTiles[tileIndex], outdoortileset5_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 6) {
				//zx7_Decompress(mapTiles[tileIndex], outdoortileset6_tiles_compressed[tileIndex]);
			}
		}
	}
}