#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tice.h>
#include <fileioc.h>

#include "savegame.h"
#include "data.h"
#include "text.h"
#include "battle.h"
#include "map.h"

char appVarName[] = "PKMNSV ";
const int version = 2;
int readVersion = 0;

struct saveGame currentSave;





void save_Save(void) {
	ti_var_t save;

	ti_CloseAll();

	save = ti_Open(appVarName, "w");

	if (save) {
		if (ti_Write(&version, sizeof(version), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&currentSave, sizeof(currentSave), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&enemyParty, sizeof(enemyParty), 1, save) != 1) {
			goto err;
		}
		if (ti_Write(&currentTrainer, sizeof(currentTrainer), 1, save) != 1) {
			goto err;
		}
		
	}
	else {
		goto err;
	}
	ti_SetArchiveStatus(true, save);
	ti_CloseAll();
	text_Display("Your progress has been saved");
	return;

err:
	ti_Delete(appVarName);
	ti_CloseAll();
	text_Display("SAVE FAILED");
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
			if (readVersion == 1) {
				if (ti_Read(&currentSave, sizeof(currentSave)-1, 1, save) != 1) {
					goto err;
				}
			}
			else {
				text_Display("This save file is from a\ndifferent version of the game.\n It cannot be loaded.");
				goto err; /* do not load if wrong version */
			}
		}
		else {
			if (ti_Read(&currentSave, sizeof(currentSave), 1, save) != 1) {
				goto err;
			}
			if (ti_Read(&enemyParty, sizeof(enemyParty), 1, save) != 1) {
				goto err;
			}
			if (ti_Read(&currentTrainer, sizeof(currentTrainer), 1, save) != 1) {
				goto err;
			}
		}
	}
	else {
		goto err;
	}
	ti_CloseAll();
	return;

err:
	/* Generate New SaveGame */
	currentSave.newGame = true;
	currentSave.rivalPokemon = 0;
	currentSave.playerX = 15 * 16;
	currentSave.playerY = 6 * 16;
	currentSave.lastPlayerX = 16 * 16;
	currentSave.lastPlayerY = 20 * 16;
	currentSave.currentZone = 0;
	currentSave.indoors = true;
	currentSave.surfing = false;
	currentSave.currentBuilding = 1;
	currentSave.playerMoney = 1000;
	currentSave.badgeCount = 0;
	currentSave.worldState = 1;
	currentSave.inFight = false;

	ti_CloseAll();
}


void save_SelectSave(void) {
	int answer;
	answer = 0;
	while (answer == 0) {
		answer = text_AskQuestion2("Save Slot 1", "Save Slot 2");
	}
	if(answer == 1){
		strcpy(appVarName, "PKMNSV1");
	}
	if (answer == 2) {
		strcpy(appVarName, "PKMNSV2");
	}
}