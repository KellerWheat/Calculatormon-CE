#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <tice.h>
#include <fileioc.h>

#include "savegame.h"
#include "data.h"
#include "text.h"

const char appVarName[] = "PKMNSV";
const int version = 1;
int readVersion = 0;

bool newGame = true;

uint16_t playerX = 29*16;
uint16_t playerY = 8*16;

uint16_t lastPlayerX = 0;
uint16_t lastPlayerY = 0;

struct pokemonData party[36];

uint16_t currentZone = 0;
bool indoors = false;
uint8_t currentBuilding = 0;

uint32_t playerMoney = 1000;
uint8_t playerItems[185] = {0};

bool defeatedTrainers[16][16] = { {0} };
bool defeatedTrainersIndoors[16][16] = { {0} };
bool takenGifts[256] = { 0 };

uint8_t badgeCount = 0;

void save_Save(void) {
	ti_var_t save;

	ti_CloseAll();

	save = ti_Open(appVarName, "w");

	if (save) {
		if (ti_Write(&version, sizeof(version), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&newGame, sizeof(newGame), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&playerX, sizeof(playerX), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&playerY, sizeof(playerY), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&lastPlayerX, sizeof(lastPlayerX), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&lastPlayerY, sizeof(lastPlayerY), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&party, sizeof(party), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&currentZone, sizeof(currentZone), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&indoors, sizeof(indoors), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&currentBuilding, sizeof(currentBuilding), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&playerMoney, sizeof(playerMoney), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&playerItems, sizeof(playerItems), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&defeatedTrainers, sizeof(defeatedTrainers), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&defeatedTrainersIndoors, sizeof(defeatedTrainersIndoors), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&takenGifts, sizeof(takenGifts), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&badgeCount, sizeof(badgeCount), 1, save) != 1) {
			goto err;
		}
	}
	else {
		goto err;
	}
	ti_SetArchiveStatus(true, save);
	ti_CloseAll();
	return;

err:
	ti_Delete(appVarName);
	ti_CloseAll();
	text_Display("SAVE FAILED", false);
}

void save_Load(void) {
	ti_var_t save;

	ti_CloseAll();

	save = ti_Open(appVarName, "r");

	if (save) {
		if (ti_Read(&readVersion, sizeof(readVersion), 1, save) != 1) {
			goto err;
		}
		if (version != readVersion) {
			goto err; /* do not load if wrong version */
		}
		if (ti_Read(&newGame, sizeof(newGame), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&playerX, sizeof(playerX), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&playerY, sizeof(playerY), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&lastPlayerX, sizeof(lastPlayerX), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&lastPlayerY, sizeof(lastPlayerY), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&party, sizeof(party), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&currentZone, sizeof(currentZone), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&indoors, sizeof(indoors), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&currentBuilding, sizeof(currentBuilding), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&playerMoney, sizeof(playerMoney), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&playerItems, sizeof(playerItems), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&defeatedTrainers, sizeof(defeatedTrainers), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&defeatedTrainersIndoors, sizeof(defeatedTrainersIndoors), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&takenGifts, sizeof(takenGifts), 1, save) != 1) {
			goto err;
		}
		if (ti_Read(&badgeCount, sizeof(badgeCount), 1, save) != 1) {
			goto err;
		}
	}
	else {
		goto err;
	}
	ti_CloseAll();
	return;

err:
	ti_CloseAll();
}