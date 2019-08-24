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
uint16_t ShiftTile(uint16_t value, uint8_t direction, uint8_t amount);
uint8_t GetTypeMapData(uint8_t tx, uint8_t ty, int width);
void DrawPlayer(void);
/* Draw 16 npcs */
void ProcessNpcs(void);
/* Loads the tilemap, typemap ,and zonedata for current area */
void LoadMap(void);
void LoadTileset(bool freeOld);

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
bool npcSaidMessage[16];

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
uint8_t jumpState = 0;
uint8_t jumpHeights[17] = {1,3,6,9,12,14,15,16,16,15,14,12,9,6,3,1, 0 };

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

	if (currentSave.indoors) {
		tilemap.width = 21;
		tilemap.height = 15;
	}

	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadMap();
	dbg_SetBreakpoint(&map_Draw);
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
		nextTile = GetNextTile(tx, ty, tilemap.width);
		if (GetNextTile(tx, ty, tilemap.width) >= 0x1A && GetNextTile(tx, ty, tilemap.width) <= 0x20) {
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
				text_Display(str,true);
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
					currentSave.party[0] = stats_NewCharacter((1 + (starter) * 3), 5);
					currentTileMap[tilePos - tilemap.width]-=3;
					currentSave.newGame = false;
					currentSave.defeatedTrainers[0][15] = true;
					currentSave.rivalPokemon = (starter + 1) % 3;
				}
				map_LoadPokeballs();
			}
			else {
				text_Display("You already chose a Pokemon", true);
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
					if (currentSave.party[partyIndex].moves[moveIndex] == 26) {
						hasUser = true;
						sprintf(str, "%s used Cut", data_pokemon[currentSave.party[partyIndex].id].name);
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
			ReadText(nextTile - 0x70);
		}
		else if (nextTile >= 0x80 && nextTile < 0x90) {
			if (!currentSave.takenGroundGifts[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][nextTile - 0x80]) {
				currentSave.takenGroundGifts[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][nextTile - 0x80] = true;
				currentSave.playerItems[currentZoneData.grounditems[nextTile - 0x80] - 1]++;
				items_IndexToName(str1, currentZoneData.grounditems[nextTile - 0x80] - 1);
				if (currentZoneData.grounditems[nextTile - 0x80] - 1 >= 20) {
					sprintf(str, "Found the TM for %s", str1);
				}
				else {
					sprintf(str, "Found a %s", str1);
				}
				text_Display(str, false);
				currentTileMap[ShiftTile(tx + tilemap.width*ty, moveDir, 1)] = 0;
				currentTypeMap[ShiftTile(tx + tilemap.width*ty, moveDir, 1)] = 0;
				currentSave.foundItemsPos[currentSave.indoors][(currentSave.currentZone * !currentSave.indoors) + (currentSave.currentBuilding * currentSave.indoors)][nextTile - 0x80] = ShiftTile(tx + tilemap.width*ty, moveDir, 1);
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
		Wait(1);
		moveState--;
		if (jumpState) {
			jumpState--;
		}
		if (running) {
			Wait(1);
			moveState--;
			if (jumpState) {
				jumpState--;
			}
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
			playerState = 0;
			/* Arrive At Tile*/
			tx = currentSave.playerX / 16;
			ty = currentSave.playerY / 16;
			nextTile = GetTypeMapData(tx, ty, tilemap.width);
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
				map_Draw();
				gfx_SwapDraw();
				if (nextTile == 0x01) {
					ExitBuilding();
				}
				else if (nextTile == 0x02) {
					if (((currentSave.indoors && !currentSave.defeatedTrainersIndoors[currentSave.currentZone][15]) || (!currentSave.indoors && !currentSave.defeatedTrainers[currentSave.currentZone][15])) && currentZoneData.trainerdir[15] != 0) {
						if (currentZoneData.trainerx[15] == 255) {
							int walkState, xloc, yloc;

							xloc = (16 * (tx - (11 * ((currentZoneData.trainerdir[15] == 1) - (currentZoneData.trainerdir[15] == 2))))) - screenX - 8;
							yloc = (16 * (ty - (7 * ((currentZoneData.trainerdir[15] == 3) - (currentZoneData.trainerdir[15] == 4))))) - screenY + 1;

							if (currentZoneData.trainerdir[15] <= 2) {
								walkState = 10 * 16;
							}
							else {
								walkState = 6 * 16;
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
							currentZoneData.trainerx[15] = tx + ((0 - currentZoneData.trainerdir[15] == 1) + (currentZoneData.trainerdir[15] == 2));
							currentZoneData.trainery[15] = ty + ((0 - currentZoneData.trainerdir[15] == 3) + (currentZoneData.trainerdir[15] == 4));


						}
						if (FightTrainer(15)) {
							currentZoneData.trainerx[15] = 255;
							currentZoneData.trainery[15] = 255;
							return 1;
						}
					}
				}
				else if (currentSave.surfing && !(nextTile >= 0x1A && nextTile < 0x20)) {
					currentSave.surfing = false;
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
			if (GetNextTile(tx, ty, tilemap.width) < 64 && !(!currentSave.surfing && GetNextTile(tx, ty, tilemap.width) >= 0x1A && GetNextTile(tx, ty, tilemap.width) < 0x20)) {
				moveState = 8;
			}
			else if (GetNextTile(tx, ty, tilemap.width) == 0x48 && moveDir == 3) {
				moveState = 8;
				jumpState = 16;
			}
		}
		running = ((kb_Data[2] & kb_Alpha) && moveState > 0 && !currentSave.surfing);
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
	if (currentSave.indoors) {
		screenX = 0;
		screenY = 0;

		gfx_Tilemap(&tilemap, screenX, screenY);

		DrawPlayer();
	}
	else {
		/* Animate Water Every 20 Frames */
		/*if (data_tileSets[currentSave.currentZone] == 0) {
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
		}*/

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

	gfx_TransparentSprite_NoClip((gfx_sprite_t*)PKMNSD8[(24 * currentSave.surfing) + (12 * running) + (moveDir - 1) * (3 - currentSave.surfing) + playerState], currentSave.playerX - screenX - 8, currentSave.playerY - screenY + 4 - jumpHeights[16-jumpState]);

	/* Draw overlay if player is in grass */
	if (currentTileMap[tx + (moveDir == 1) - (moveDir == 2) + (ty + (moveDir == 3) - (moveDir == 4)) * tilemap.width] == 2 && !currentSave.indoors) {
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
	if (!(moveDir == 3 && moveState != 0) && currentTileMap[tx + ty * tilemap.width] == 2) {
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
	text_Display("Your currentSave.party has been healed", false);
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
	text_Display(loadedText, true);
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
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadTileset(true);
	LoadMap();
}
void ExitZone(uint8_t index) {
	currentSave.playerX = currentZoneData.exitx[index] * 16;
	currentSave.playerY = currentZoneData.exity[index] * 16;
	currentSave.currentZone = currentZoneData.exitzone[index];
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadTileset(true);
	LoadMap();
}
bool FightTrainer(uint8_t index) {
	if (currentZoneData.trainertype[index] <=1) {
		currentTrainer = index;
		if (currentSave.indoors && !currentSave.defeatedTrainersIndoors[currentSave.currentBuilding][currentTrainer]) {
			lastTextIndex = 0;
			lastTextID = currentZoneData.trainertext[index];
			LoadText(currentZoneData.trainertext[index]);
			text_Display(loadedText, true);
			battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
			return true;
		}
		else if (!currentSave.indoors && !currentSave.defeatedTrainers[currentSave.currentZone][currentTrainer]) {
			lastTextIndex = 0;
			lastTextID = currentZoneData.trainertext[index];
			LoadText(currentZoneData.trainertext[index]);
			text_Display(loadedText, true);
			battle_SpawnTrainer(currentZoneData.trainerspawnids, currentZoneData.trainerspawnlevels, index);
			return true;
		}
		
	}
	else if (currentZoneData.trainertype[index] <= 3) {
		LoadText(currentZoneData.trainertext[index]);
		text_Display(loadedText, true);
	}
	else if (currentZoneData.trainertype[index] == 4 && moveState == 0) {
		LoadText(currentZoneData.trainertext[index]);
		text_Display(loadedText, true);
		moveDir = moveDir + 1 - 2 * !(moveDir % 2);
		moveState = 8;
	}
	else if (currentZoneData.trainertype[index] == 5 && moveState == 0) {
		LoadText(currentZoneData.trainertext[index]);
		text_Display(loadedText, true);
		moveDir = moveDir + 1 - 2 * !(moveDir % 2);
		moveState = 8;
	}
	else if (currentZoneData.trainertype[index] == 6) {
		if (currentSave.newGame) {
			text_Display("Professor Spruce was looking for you\nHe is in his Laboratory", false);
		}
		else {
			if (!currentSave.takenGiftsIndoors[currentSave.currentBuilding][index]) {
				text_Display("You can use these to catch\nmore Pokemon\n\n\nRecieved 5 pokeballs", true);
				currentSave.playerItems[0] += 5;
				currentSave.takenGiftsIndoors[currentSave.currentBuilding][index] = true;
			}
			else {
				text_Display("Good luck on your adventure!", true);
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
		ti_Seek(((int)currentSave.currentBuilding) * 1318, 0, mapAV);
		ti_Read(&currentTileMap, 315, 1, mapAV);
		ti_Read(&currentTypeMap, 315, 1, mapAV);
		ti_Read(&currentZoneData, sizeof(currentZoneData), 1, mapAV);
	}
	else {
		mapAV = ti_Open("PKMNMD0", "r");
		ti_Seek(((int)currentSave.currentZone) * 4528, 0, mapAV);
		ti_Read(&currentTileMap, 1920, 1, mapAV);
		ti_Read(&currentTypeMap, 1920, 1, mapAV);
		ti_Read(&currentZoneData, sizeof(currentZoneData), 1, mapAV);
	}
	ti_CloseAll();
	for (npcIndex = 0; npcIndex < 16; npcIndex++) {
		if (currentZoneData.trainertype[npcIndex] == 4) {
			bool canPass = true;
			if (currentSave.newGame && currentZoneData.spawnids[0][npcIndex] == 0) {
				canPass = false;
			}
			if (currentZoneData.spawnids[0][npcIndex] > playerState) {
				canPass = false;
			}
			if (canPass) {
				currentZoneData.trainerdir[npcIndex] = 0;
			}
		}


		npcMoveState[npcIndex] = 0;
		npcGrassAnimState[npcIndex] = 0;
		npcOldTiles[npcIndex] = currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tilemap.width * currentZoneData.trainery[npcIndex])];
		currentTypeMap[(currentZoneData.trainerx[npcIndex]) + (tilemap.width * currentZoneData.trainery[npcIndex])] = 96 + npcIndex;
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
	return (value + ((direction == 1) * amount) - ((direction == 2) * amount) + ((direction == 3) * tilemap.width * amount) - ((direction == 4) * tilemap.width * amount));
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
						if (currentZoneData.trainertype[npcIndex] == 0 || (currentZoneData.trainertype[npcIndex] == 2 && !npcSaidMessage[npcIndex]) || currentZoneData.trainertype[npcIndex] == 4 || (currentZoneData.trainertype[npcIndex] == 6 && !npcSaidMessage[npcIndex] && currentSave.newGame)) {
							npcSaidMessage[npcIndex] = true;
							npcToFight = npcIndex;
						}
						break;
					}
				}
				if (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 1 && npcMoveState[npcIndex] % 16 == 8) {
					currentZoneData.trainerdir[npcIndex] = 5 - trainerDir;
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


				if (currentZoneData.trainerskin[npcIndex] < 128) {
					gfx_TransparentSprite((gfx_sprite_t*)PKMNSD7[(currentZoneData.trainerskin[npcIndex] * 12) + ((trainerDir - 1) * 3) + ((npcMoveState[npcIndex] % 16) / 4 == 1 && (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove)) + 2 * ((npcMoveState[npcIndex] % 16) / 4 == 3 && (npcPatterns[currentZoneData.trainermovement[npcIndex]][npcMoveState[npcIndex] / 16] == 4 && canMove))], xloc + xlocMod - 8, yloc + ylocMod + 3);
				}
				else {
					gfx_TransparentSprite((gfx_sprite_t*)PKMNSD7[36 + ((currentZoneData.trainerskin[npcIndex]-128) * 4) + (trainerDir - 1)], xloc + xlocMod - 8, yloc + ylocMod + 3);
				}
				
				/* Grass */
				if (currentTileMap[currentZoneData.trainerx[npcIndex] + tilemap.width * currentZoneData.trainery[npcIndex]] == 2 && !currentSave.indoors) {
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
	text_Display("All your pokemon have fainted", false);
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
	tx = currentSave.playerX / 16;
	ty = currentSave.playerY / 16;
	LoadMap();
}

void map_WinFight(bool wild, uint16_t rewardMoney) {
	char str1[16];
	if (!wild) {
		if (lastTextIndex > 0) {
			map_Draw();
			LoadText(lastTextID);
			text_Display(loadedText + lastTextIndex , true);
		}
		currentSave.playerMoney += rewardMoney;
		if (currentSave.indoors) {
			currentSave.defeatedTrainersIndoors[currentSave.currentBuilding][currentTrainer] = true;
			if (currentZoneData.trainerreward[currentTrainer] != 0) {
				if (currentZoneData.trainerreward[currentTrainer] == 255) {
					currentSave.badgeCount++;
					sprintf(str, "Recieved a badge");
				}
				else {
					currentSave.playerItems[currentZoneData.trainerreward[currentTrainer] - 1]++;
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
			currentSave.defeatedTrainers[currentSave.currentZone][currentTrainer] = true;
			if (currentZoneData.trainerreward[currentTrainer] != 0) {
				currentSave.playerItems[currentZoneData.trainerreward[currentTrainer] - 1]++;
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
void LoadTileset(bool freeOld) {
	int tileIndex = 0;
	for (tileIndex = 0; tileIndex < 128; tileIndex++) {
		if (freeOld) {
			free(mapTiles[tileIndex]);
		}
		mapTiles[tileIndex] = gfx_MallocSprite(16, 16);
		if (currentSave.indoors) {
			zx7_Decompress(mapTiles[tileIndex], indoortileset0_tiles_compressed[tileIndex]);
		}
		else {
			if (data_tileSets[currentSave.currentZone] == 0) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset0_tiles_compressed[tileIndex]);
			}
			else if (data_tileSets[currentSave.currentZone] == 1) {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset1_tiles_compressed[tileIndex]);
			}
			else {
				zx7_Decompress(mapTiles[tileIndex], outdoortileset2_tiles_compressed[tileIndex]);
			}
		}
	}
}