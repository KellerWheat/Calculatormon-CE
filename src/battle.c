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
#include <fileioc.h>
#include <debug.h>
#include <compression.h>

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
#include "gfx/PKMNSD6.h"

void SetupBattleGfx(void);

bool PlayerTurn(void);
bool EnemyTurn(void);
int SelectMove(void);
void RedrawCharacters(uint8_t hide);
void Attack(bool player, uint8_t move);
uint8_t CalculateXpPercent(void);
void ApplyStatus(bool player);
void ResetStatus(bool player);
void TakeDamage(bool player, int amount);
void Heal(bool player, int amount);
bool Capture(uint8_t ball);
void AddXp(void);
void AnimateDamage(bool player);
void DrawHealthBar(void);


const int statModValues[] = { 25,29,33,40,50,67,100,150,200,250,300,350,400 };
const struct pokemonData clearCharacter = { 0 };

uint8_t currentPlayer;
uint8_t currentEnemy;

char playerName[20];
char enemyName[20];

gfx_sprite_t *playerSprite;
gfx_sprite_t *enemySprite;

gfx_sprite_t *backgroundSprite;
gfx_sprite_t *hpBarSprite;
gfx_sprite_t *statusSprites[5];

struct pokemonData enemyParty[6];
uint8_t attackTurn = 0;
uint8_t chosenMove[2];
bool wild = false;

struct pokemonStats stats[2];
uint8_t *currentStatusPointer[2];

/* Modifiers for stats; can only be changed 6 times the nmove will fail*/
uint8_t statMods[2][7];
/* Confusion gives a chance to hurt to a pokemon instead of attacking */
uint8_t confusedTurns[2];
/* Steals life each turn */
bool leech[2];
uint8_t trapTurns[2];
uint8_t reflectTurns[2];
uint8_t lightScreenTurns[2];
uint8_t mistTurns[2];
bool flinch[2];
/* gives an 8x crit chance */
bool highCritRatio[2];
bool rage[2];
bool transformed[2];
bool converted[2];
struct pokemonData originalpokemon[2];
uint16_t bideDamage[2];
uint16_t substituteHealth[2];
uint8_t disabledMove[2];
uint8_t disabledTurns[2];
uint8_t lastMove[2];
uint8_t attackTurns[2];
bool air[2];

uint16_t payday;
bool ft;
bool run;

uint8_t battleMenuState1 = 0;
uint8_t battleMenuState2 = 0;
uint8_t battleMenuCurrent = 1;
bool textExit = false;//This allow the Sto Key to make AskQuestion4() return 5

const uint8_t statuscolors[5] = { 2,3,4,5,6 };

void battle_Initialize(void) {

}
void battle_Setup(void) {
	int pokemonIndex;
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {//This makes sure the player pokemon sent out has not fainted
		if (currentSave.party[pokemonIndex].currenthealth != 0 && currentSave.party[pokemonIndex].id != 0) {
			currentPlayer = pokemonIndex;
			pokemonIndex = 6;
		}
	}
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {//This makes sure the enemy pokemon sent out has not fainted
		if (enemyParty[pokemonIndex].currenthealth != 0 && enemyParty[pokemonIndex].id != 0) {
			currentEnemy = pokemonIndex;
			pokemonIndex = 6;
		}
	}
	SetupBattleGfx();

	battleMenuState1 = 0;
	battleMenuState2 = 0;
	battleMenuCurrent = 1;

	
	stats[0] = stats_CalculateStats(enemyParty[0]);
	stats[1] = stats_CalculateStats(currentSave.party[0]);
	attackTurn = 0;

	ResetStatus(true);
	ResetStatus(false);
	payday = 0;
	ft = true;

	run = false;
}
void SetupBattleGfx(void) {
	textBoxType = 1;
	backgroundSprite = gfx_MallocSprite(160, 88);
	zx7_Decompress(backgroundSprite, background_compressed);
	hpBarSprite = gfx_MallocSprite(98, 10);
	zx7_Decompress(hpBarSprite, hpbar_compressed);
	playerSprite = gfx_MallocSprite(56, 56);
	enemySprite = gfx_MallocSprite(56, 56);

	if (currentSave.party[currentPlayer].id < 80) {
		zx7_Decompress(playerSprite, PKMNSD2[currentSave.party[currentPlayer].id]);
	}
	else {
		zx7_Decompress(playerSprite, PKMNSD3[currentSave.party[currentPlayer].id - 80]);
	}
	if (enemyParty[currentEnemy].id < 80) {
		zx7_Decompress(enemySprite, PKMNSD0[enemyParty[currentEnemy].id]);
	}
	else {
		zx7_Decompress(enemySprite, PKMNSD1[enemyParty[currentEnemy].id - 80]);
	}
	

	MallocIcons();

	SetColors(1);
	gfx_FillScreen(colors[0]);
	gfx_SetPalette(battle_gfx_pal, sizeof_battle_gfx_pal, 0);
	gfx_SwapDraw();
	gfx_SetTextScale(1, 1);
	gfx_SetTextFGColor(colors[1]);
	gfx_SetColor(colors[0]);
}
int battle_Loop(void) {
	/* Battle */
	RedrawCharacters(0);
	/* Switch to a new poketmon or lose battle if health is 0 */
	if (currentSave.party[currentPlayer].currenthealth == 0) {
		sprintf(str, "%s fainted", playerName);
		text_Display(str);
		attackTurn = 0;

		if (transformed[true]) {
			currentSave.party[currentPlayer] = originalpokemon[true];
		}
		if (converted[true]) {
			data_pokemon[currentSave.party[currentPlayer].id].element1 = 1;
		}

		i = 0;
		while (i < 6) {
			if (currentSave.party[i].currenthealth != 0 && currentSave.party[i].id != 0) {
				currentPlayer = i;
				ResetStatus(true);
				return 1;
			}
			i++;
		}

		map_LoseFight();
		return 0;
	}
	if (enemyParty[currentEnemy].currenthealth == 0) {
		sprintf(str, "%s fainted", enemyName);
		text_Display(str);
		AddXp();
		attackTurn = 0;

		if (transformed[false]) {
			enemyParty[currentEnemy] = originalpokemon[false];
		}
		if (converted[false]) {
			data_pokemon[enemyParty[currentEnemy].id].element1 = 1;
		}

		i = 0;
		while (i < 6) {
			if (enemyParty[i].currenthealth != 0 && enemyParty[i].id != 0) {
				currentEnemy = i;
				ResetStatus(false);
				return 1;
			}
			i++;
		}

		currentSave.playerMoney += payday;
		map_WinFight(wild, enemyParty[0].level * 40);
		return 0;
	}
	/* If 0 determine who Attacks first, if 1 or 2 let other character Attack, if 3 do end of turn stuff */
	if (attackTurn == 0) {
		while (!PlayerTurn()) {};
		if (currentSave.inFight == true) {
			save_Save();
			return 2;
		}
		kb_Scan();
		while (!EnemyTurn()) {};
		if (run) {
			return 0;
		}
		if ((kb_Data[6] & kb_Div) && debugging) {
			enemyParty[currentEnemy].currenthealth = 0;
		}
		else {
			/* Paralysis gets a speed reduction */
			if ((((stats[0].speed / (1 + (((*currentStatusPointer[0]) == 4) * 3))) > (stats[1].speed / (1 + (((*currentStatusPointer[1]) == 4) * 3)))) || data_moves[chosenMove[false]].statustype == 31) && data_moves[chosenMove[true]].statustype != 31) {
				Attack(false, chosenMove[false]);
				attackTurn = 1;
			}
			else {
				Attack(true, chosenMove[true]);
				attackTurn = 2;
			}
			if (run) {
				return 0;
			}
		}
	}
	else if(attackTurn < 3) {
		if (attackTurn == 1) {
			Attack(true, chosenMove[true]);
			attackTurn = 3;
		}
		if (attackTurn == 2) {
			Attack(false, chosenMove[false]);
			attackTurn = 3;
		}
	}
	else {
		/* After both turns */
		ApplyStatus(true);
		ApplyStatus(false);
		flinch[true] = false;
		flinch[false] = false;
		attackTurn = 0;
		ft = false;
		return 1;
	}
	

	return 1;
}
void battle_End(void) {
	free(playerSprite);
	free(enemySprite);
	free(backgroundSprite);
	free(hpBarSprite);
	FreeIcons();
}

/* Sets up a battle with a wild pokemon */
void battle_SpawnWild(uint8_t id, uint8_t minlevel, uint8_t maxlevel) {
	int partyIndex;
	for(partyIndex = 0; partyIndex < 6; partyIndex++){
		enemyParty[partyIndex] = clearCharacter;
	}
	enemyParty[0] = stats_NewCharacter(id,(rand() % (maxlevel-minlevel+1))+minlevel);
	wild = true;
}
/* Sets up a battle with a trainer */
void battle_SpawnTrainer(uint8_t ids[6][16], uint8_t levels[6][16], uint8_t trainernum) {
	int partyIndex;
	for(partyIndex = 0; partyIndex < 6; partyIndex++){
		enemyParty[partyIndex] = stats_NewCharacter(ids[partyIndex][trainernum],levels[partyIndex][trainernum]);
	}
	wild = false;
}

bool PlayerTurn() {
	if (attackTurns[true] > 0) {
		chosenMove[true] = lastMove[true];
		return true;
	}
	i = 0;
	/* Select Whether to fight, run, ect */
	textExit = true;
	if (battleMenuState1 == 0) {
		switch (text_AskQuestion4("Fight","Item","Switch","Run"))
		{
		case 0:
			return false;
		case 1:
			battleMenuState1 = 1;
			return false;
		case 2:
			battleMenuState1 = 2;
			return false;
		case 3:
			battle_End();
			i = menu_PokemonMenu(false);
			SetupBattleGfx();
			if (i != 0 && (i - 1) != currentPlayer && currentSave.party[i - 1].id != 0) {
				currentPlayer = i - 1;
				ResetStatus(true);
				RedrawCharacters(0);
				return true;
			}
			RedrawCharacters(0);
			return false;
		case 4:
			if (!wild) {
				text_Display("Can't run from trainers!");
				return false;
			}
			//NOT REAL RUN FUNCTION
			if (stats[1].speed > stats[0].speed || (rand() % 3) == 0) {
				run = true;
				return true;
			}
			else {
				text_Display("Failed to run away!");
				return true;
			}
			return false;
		case 5:
			text_Display("Would you like to save and\nexit the battle?");
			if (text_AskQuestion2("Yes", "No") == 1) {
				currentSave.inFight = true;
				return 1;
			}
			return false;
		}
		
	}
	/* Select which move to use */
	if (battleMenuState1 == 1) {
		int selectedMove;
		selectedMove = SelectMove();
		battleMenuState1 = 0;
		/* Checks if the move can be used */
		if (selectedMove == 0 || currentSave.party[currentPlayer].pp[selectedMove - 1] == 0 || disabledMove[true] == selectedMove) {
			return false;
		}
		chosenMove[true] = currentSave.party[currentPlayer].moves[selectedMove - 1];
		currentSave.party[currentPlayer].pp[selectedMove - 1]--;
	}
	/* Items */
	else if (battleMenuState1 == 2) {
		int chosenItem;
		battleMenuState1 = 0;
		battle_End();
		chosenItem = menu_Items(true);
		SetupBattleGfx();
		RedrawCharacters(0);
		if (chosenItem == -1) {
			return false;
		}
		if (chosenItem < 3 || (chosenItem >= 20 && chosenItem < 26) ){
			sprintf(str, "Used a %s", itemNames[chosenItem]);
			text_Display(str);
			currentSave.playerItems[chosenItem]--;
			chosenMove[true] = 0;
			if (Capture(chosenItem)) {
				if (transformed[1]) {
					enemyParty[1] = originalpokemon[1];
				}
				map_WinFight(wild, enemyParty[0].level * 40);
				run = true;
				return true;
			}
		}
		else {
			battle_End();
			if (items_UseItem(chosenItem)) {
				chosenMove[true] = 0;
				SetupBattleGfx();
				RedrawCharacters(0);
			}
			else {
				SetupBattleGfx();
				RedrawCharacters(0);
				return false;
			}
		}
	}
	return true;
}
int SelectMove(void) {
	int8_t tv1, tv2, iconIndex;
	tv1 = 0;
	tv2 = 0;

	textBoxType = 2;
	gfx_Blit(gfx_screen);
	text_DrawTextBox();



	gfx_PrintStringXY(">", 15, 190);
	gfx_PrintStringXY(data_moves[currentSave.party[currentPlayer].moves[0]].name, 25, 190);
	gfx_PrintStringXY(data_moves[currentSave.party[currentPlayer].moves[1]].name, 25, 205);
	gfx_PrintStringXY(data_moves[currentSave.party[currentPlayer].moves[2]].name, 145, 190);
	gfx_PrintStringXY(data_moves[currentSave.party[currentPlayer].moves[3]].name, 145, 205);

	if (currentSave.party[currentPlayer].moves[tv1 + 2 * tv2] != 0) {
		gfx_Sprite(typeIcons[data_moves[currentSave.party[currentPlayer].moves[tv1 + 2 * tv2]].element - 1], 271, 193);
		sprintf(str, "%u/%u", currentSave.party[currentPlayer].pp[tv1 + 2 * tv2], data_moves[currentSave.party[currentPlayer].moves[tv1 + 2 * tv2]].uses);
		gfx_PrintStringXY(str, 271, 209);
	}
	

	gfx_SwapDraw();
	gfx_SetDrawScreen();

	kb_Scan();
	if ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) {
		Wait(50);
	}
	kb_Scan();
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
		if ((kb_Data[7] & kb_Down) || (kb_Data[7] & kb_Up)) {
			if (tv1 == 1) {
				tv1 = 0;
			}
			else {
				tv1 = 1;
			}
			gfx_FillRectangle(14, 185, 10, 45);
			gfx_FillRectangle(134, 185, 10, 45);
			gfx_PrintStringXY(">", 15 + tv2 * 120, 190 + 15 * tv1);

			gfx_FillRectangle(263, 185, 48, 46);
			if (currentSave.party[currentPlayer].moves[tv1 + 2 * tv2] != 0) {
				gfx_Sprite(typeIcons[data_moves[currentSave.party[currentPlayer].moves[tv1 + 2 * tv2]].element - 1], 271, 193);
				sprintf(str, "%u/%u", currentSave.party[currentPlayer].pp[tv1 + 2 * tv2], data_moves[currentSave.party[currentPlayer].moves[tv1 + 2 * tv2]].uses);
				gfx_PrintStringXY(str, 271, 209);
			}

			Wait(20);
		}
		if ((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
			if (tv2 == 1) {
				tv2 = 0;
			}
			else {
				tv2 = 1;
			}
			gfx_FillRectangle(14, 185, 10, 45);
			gfx_FillRectangle(134, 185, 10, 45);
			gfx_PrintStringXY(">", 15 + tv2 * 120, 190 + 15 * tv1);

			gfx_FillRectangle(263, 185, 48, 46);
			if (currentSave.party[currentPlayer].moves[tv1 + 2 * tv2] != 0) {
				gfx_Sprite(typeIcons[data_moves[currentSave.party[currentPlayer].moves[tv1 + 2 * tv2]].element - 1], 271, 193);
				sprintf(str, "%u/%u", currentSave.party[currentPlayer].pp[tv1 + 2 * tv2], data_moves[currentSave.party[currentPlayer].moves[tv1 + 2 * tv2]].uses);
				gfx_PrintStringXY(str, 271, 209);
			}

			Wait(20);
		}
	}
	if (kb_Data[6] & kb_Clear) {
		textBoxType = 1;
		return(0);
	}
	gfx_SetDrawBuffer();
	textBoxType = 1;
	return (tv1 + 1 + 2 * (tv2));
}
bool EnemyTurn() {
	if (attackTurns[false] > 0) {
		chosenMove[false] = lastMove[false];
		return true;
	}
	switch (rand() % 4)
	{
	case 0:
		if (enemyParty[currentEnemy].moves[0] == 0 || disabledMove[false] == 1) {
			return false;
		}
		chosenMove[false] = enemyParty[currentEnemy].moves[0];
		break;
	case 1:
		if (enemyParty[currentEnemy].moves[1] == 0 || disabledMove[false] == 2) {
			return false;
		}
		chosenMove[false] = enemyParty[currentEnemy].moves[1];
		break;
	case 2:
		if (enemyParty[currentEnemy].moves[2] == 0 || disabledMove[false] == 3) {
			return false;
		}
		chosenMove[false] = enemyParty[currentEnemy].moves[2];
		break;
	case 3:
		if (enemyParty[currentEnemy].moves[3] == 0 || disabledMove[false] == 4) {
			return false;
		}
		chosenMove[false] = enemyParty[currentEnemy].moves[3];
		break;
	}
	return true;
}

void RedrawCharacters(uint8_t hide) {
	int statusIndex, healthRatio;
	gfx_SetColor(colors[1]);
	text_DrawTextBox();

	gfx_ScaledSprite_NoClip(backgroundSprite, 0, 0, 2, 2);
	if (hide != 2) {
		gfx_ScaledTransparentSprite_NoClip(playerSprite, 20, 64, 2, 2);
	}
	if (hide != 1) {
		gfx_ScaledTransparentSprite_NoClip(enemySprite, 190, 8, 2, 2);
	}

	/* Display Information about player and enemy */
	
	DrawHealthBar();

	sprintf(str, "%u/%u", currentSave.party[currentPlayer].currenthealth, stats[1].health);
	gfx_PrintStringXY(str, 247, 153);

	gfx_SetTextFGColor(colors[11]);
	gfx_PrintStringXY("XP", 193, 166);
	gfx_SetColor(colors[7]);
	if (currentSave.party[currentPlayer].level < 100) {
		gfx_FillRectangle(210, 168, CalculateXpPercent(), 4);
	}
	if (currentSave.party[currentPlayer].currentstatus > 0) {
		gfx_TransparentSprite_NoClip(statusIcons[currentSave.party[currentPlayer].currentstatus - 1], 182, 141);
	}

	gfx_SetColor(colors[0]);
	gfx_SetTextFGColor(colors[1]);
	if (hide != 3) {
		gfx_SwapDraw();
	}
}

uint8_t CalculateXpPercent(void) {
	int cXp, sXp, mXp;
	cXp = currentSave.party[currentPlayer].xp;
	sXp = xpPerLevel[data_pokemon[currentSave.party[currentPlayer].id].xptype][currentSave.party[currentPlayer].level];
	mXp = xpPerLevel[data_pokemon[currentSave.party[currentPlayer].id].xptype][currentSave.party[currentPlayer].level + 1];
	return((uint8_t)((92 * (cXp - sXp)) / (mXp - sXp)));

}

void Attack(bool player, uint8_t move) {
	char username[20];
	char nonusername[20];
	uint32_t damage;
	uint8_t hitcount = 0;
	uint8_t userlevel;
	uint8_t nonuserlevel;

	uint8_t userelement1;
	uint8_t userelement2;
	uint8_t nonuserelement1;
	uint8_t nonuserelement2;

	if (move == 0) {
		return;
	}
	
	
	rage[player] = false;

	if (player) {
		strcpy(username, playerName);
		strcpy(nonusername, enemyName);
		userlevel = currentSave.party[currentPlayer].level;
		nonuserlevel = enemyParty[currentEnemy].level;
		userelement1 = data_pokemon[currentSave.party[currentPlayer].id].element1;
		userelement2 = data_pokemon[currentSave.party[currentPlayer].id].element2;
		nonuserelement1 = data_pokemon[enemyParty[currentEnemy].id].element1;
		nonuserelement2 = data_pokemon[enemyParty[currentEnemy].id].element2;

	}
	else {
		strcpy(username, enemyName);
		strcpy(nonusername, playerName);
		userlevel = enemyParty[currentEnemy].level;
		nonuserlevel = currentSave.party[currentPlayer].level;
		userelement1 = data_pokemon[enemyParty[currentEnemy].id].element1;
		userelement2 = data_pokemon[enemyParty[currentEnemy].id].element2;
		nonuserelement1 = data_pokemon[currentSave.party[currentPlayer].id].element1;
		nonuserelement2 = data_pokemon[currentSave.party[currentPlayer].id].element2;
	}

	/* Dont Attack if previous Attack made player flinch*/
	if (flinch[player]) {
		sprintf(str, "%s flinched", username);
		text_Display(str);
	}
	/* Random Move */
	if (data_moves[move].type == 16) {
		sprintf(str, "%s used %s", username, data_moves[move].name);
		text_Display(str);
		move = 1 + (rand() % 165);
	}
	/* Copy enemy move */
	if (data_moves[move].type == 17) {
		sprintf(str, "%s used %s", username, data_moves[move].name);
		text_Display(str);
		move = lastMove[!player];
	}
	lastMove[player] = move;
	/* If they used counter but they enemy did not Attack, the Attack fails */
	if (data_moves[move].type == 19 && data_moves[lastMove[!player]].type != 0) {
		text_Display("It Failed");
		return;
	}
	/* Handle turns for moves with multiple Attacks */
	if (data_moves[move].statustype == 32) {
		if (attackTurns[player] == 0) {
			attackTurns[player] = (rand() % 2) + 2;
		}
		attackTurns[player]--;
	}
	if (data_moves[move].statustype == 33 || data_moves[move].statustype == 36 || data_moves[move].statustype == 37 || data_moves[move].statustype == 38) {
		if (attackTurns[player] == 0) {
			attackTurns[player] = 1;
			sprintf(str, "%s is building up power", username, data_moves[move].name);
			text_Display(str);
			return;
		}
		else if (attackTurns[player] == 1) {
			attackTurns[player] = 0;
		}
	}
	if (data_moves[move].statustype == 34) {
		if (attackTurns[player] == 0) {
			attackTurns[player] = 1;
			air[player] = true;
			return;
		}
		else if (attackTurns[player] == 1) {
			attackTurns[player] = 0;
			air[player] = false;
		}
	}
	if (data_moves[move].type == 20 && attackTurns[player] != 1) {
		if (attackTurns[player] == 0) {
			bideDamage[player] = 0;
			attackTurns[player] = 2;
		}
		else {
			attackTurns[player]--;
		}
		sprintf(str, "%s is storing energy", username);
		text_Display(str);
		return;
	}

	/* Exit if character's status prevents movement */
	if ((*currentStatusPointer[player] == 3) && (rand() % 4 == 1)) {
		sprintf(str, "%s is paralysed", username);
		text_Display(str);
		return;
	}
	if (*currentStatusPointer[player] == 4) {
		if (rand() % 3 == 0) {
			*currentStatusPointer[player] = 0;
			sprintf(str, "%s woke up", username);
			text_Display(str);
		}
		else {
			sprintf(str, "%s is asleep", username);
			text_Display(str);
			return;
		}
	}
	if (*currentStatusPointer[player] == 5) {
		if (rand() % 5 == 0) {
			*currentStatusPointer[player] = 0;
			sprintf(str, "%s thawed out", username);
			text_Display(str);
		}
		else {
			sprintf(str, "%s is frozen", username);
			text_Display(str);
			return;
		}
	}
	if (confusedTurns[player] > 0) {
		confusedTurns[player] --;
		if (rand() % 2 == 0) {
			sprintf(str, "%s hurt itself in its confusion", username);
			text_Display(str);
			return;
		}
	}

	/* Show text for using the move */
	sprintf(str, "%s used %s", username, data_moves[move].name);
	text_Display(str);

	/* Attack might miss */
	if (air[!player] || (data_moves[move].accuracy * statModValues[statMods[player][5]] / statModValues[statMods[!player][6]]) < (rand() % 100)) {
		text_Display("It Missed");
		if (data_moves[move].statustype == 25) {
			TakeDamage(player, stats[player].health);
		}
		return;
	}
	if (!(*currentStatusPointer[player] == 4) && data_moves[move].statustype == 26) {
		text_Display("It Failed");
	}
	/* Multi-hit Attacks*/
	if (data_moves[move].statustype == 22) {
		hitcount = (rand() % 3) + 1;
	}
	if (data_moves[move].statustype == 23) {
		hitcount = 1;
	}

	


startAttack:
	/* Normal Attacks */
	if (data_moves[move].type < 2 || data_moves[move].type == 19) {
		if (data_moves[move].type == 0) {
			damage = ((((2 * userlevel / 5) + 2)*data_moves[move].power*(stats[player].attack*statModValues[statMods[player][0]] / 100) / (stats[!player].defence * statModValues[statMods[!player][1]] / 100)) / 50) + 2;
		}
		else if (data_moves[move].type == 1) {
			damage = ((((2 * userlevel / 5) + 2)*data_moves[move].power*(stats[player].specialattack*statModValues[statMods[player][2]] / 100) / (stats[!player].specialdefence * statModValues[statMods[!player][3]] / 100)) / 50) + 2;
		}
		else {
			damage = ((((2 * userlevel / 5) + 2) * data_moves[lastMove[!player]].power * 2 * (stats[player].attack*statModValues[statMods[player][0]] / 100) / (stats[!player].defence * statModValues[statMods[!player][1]] / 100)) / 50) + 2;
		}
		damage *= ((rand() % 16) + 85);//  /100
		damage *= elements[data_moves[move].element][nonuserelement1];// /2
		damage *= elements[data_moves[move].element][nonuserelement2];// /2
		damage /= 400;
		if (data_moves[move].element == userelement1 || data_moves[move].element == userelement2) {
			damage *= 1.5;
		}

		/* Critical Hit  */
		if (data_moves[move].statustype == 24 || highCritRatio[player] || data_moves[move].statustype == 36) {
			if (player) {
				if ((data_pokemon[currentSave.party[currentPlayer].id].basespeed * 4) > (rand() % 256)) {
					damage *= (((2 * userlevel) + 5) / (userlevel + 5));
					text_Display("Critical Hit");
				}
			}
			else {
				if ((data_pokemon[enemyParty[currentEnemy].id].basespeed * 4) > (rand() % 256)) {
					damage *= (((2 * userlevel) + 5) / (userlevel + 5));
					text_Display("Critical Hit");
				}
			}
		}
		else {
			if (player) {
				if ((data_pokemon[currentSave.party[currentPlayer].id].basespeed / 2) > (rand() % 256)) {
					damage *= (((2 * userlevel) + 5) / (userlevel + 5));
					text_Display("Critical Hit");
				}
			}
			else {
				if ((data_pokemon[enemyParty[currentEnemy].id].basespeed / 2) > (rand() % 256)) {
					damage *= (((2 * userlevel) + 5) / (userlevel + 5));
					text_Display("Critical Hit");
				}
			}
		}
		AnimateDamage(!player);
		TakeDamage(!player, damage);

		if (data_moves[move].statustype == 21 || data_moves[move].statustype == 26) {
			Heal(player, damage / 2);
		}
		/* Recoil */
		if (data_moves[move].statustype == 19) {
			sprintf(str, "%s is hurt by recoil", username);
			text_Display(str);
			TakeDamage(player, damage / 3);
		}
		/* Restart if Attack hits multiple times */
		if (hitcount > 0) {
			hitcount--;
			Wait(15);
			goto startAttack;
		}

		/* Display supereffective or not very effective */
		if (elements[data_moves[move].element][nonuserelement1] * elements[data_moves[move].element][nonuserelement2] < 4) {
			text_Display("Not Very Effective");
		}
		if (elements[data_moves[move].element][nonuserelement1] * elements[data_moves[move].element][nonuserelement2] > 4) {
			text_Display("Super Effective");
		}
		if (rage[!player]) {
			sprintf(str, "%s is hurt by recoil", username);
			statMods[!player][0]++;
		}
	}
	/* Raise user stat */
	else if (data_moves[move].type == 2) {
		if (statMods[player][data_moves[move].statustype] + data_moves[move].power > 12) {
			statMods[player][data_moves[move].statustype] = 12;
			text_Display("It failed");
		}
		else {
			statMods[player][data_moves[move].statustype] += data_moves[move].power;
			sprintf(str, "%s's %s rose", username, text_status2[data_moves[move].statustype]);
			text_Display(str);
		}
		
	}
	/* Lower non-user stat */
	else if (data_moves[move].type == 3) {
		if (statMods[!player][data_moves[move].statustype] < data_moves[move].power) {
			statMods[!player][data_moves[move].statustype] = 0;
			text_Display("It failed");
		}
		else {
			statMods[!player][data_moves[move].statustype] -= data_moves[move].power;
			sprintf(str, "%s's %s fell", nonusername, text_status2[data_moves[move].statustype]);
			text_Display(str);
		}
		
	}
	else if (data_moves[move].type == 5) {
		TakeDamage(!player, data_moves[move].power);
	}
	else if (data_moves[move].type == 6 || data_moves[move].type == 7) {
		if (wild) {
			run = true;
		}
		else {
			text_Display("It Failed");
		}
	}
	else if (data_moves[move].type == 8) {
		Heal(player, stats[player].health / 2);
	}
	else if (data_moves[move].type == 9) {
		TakeDamage(!player, userlevel);
	}
	else if (data_moves[move].type == 10) {
		TakeDamage(!player, userlevel * 20 / (10 + (rand() % 20)));
	}
	else if (data_moves[move].type == 11) {
		TakeDamage(!player, stats[!player].health / 2);
	}
	else if (data_moves[move].type == 12) {
		i = 0;
		while (i < sizeof(statMods[player])) {
			statMods[player][i] = 6;
			i++;
		}
		while (i < sizeof(statMods[!player])) {
			statMods[player][i] = 6;
			i++;
		}
	}
	else if (data_moves[move].type == 13) {
		*currentStatusPointer[player] = 4;
		Heal(player, 1000);
	}
	else if (data_moves[move].type == 14) {
		
		disabledTurns[!player] = (rand() % 7) + 2;
		disabledMove[!player] = lastMove[!player];
	}
	else if (data_moves[move].type == 15) {
		if ((int)userlevel - (int)nonuserlevel + 30 > (rand() % 100)) {
			TakeDamage(!player, 800);
		}
		else {
			text_Display("It Missed");
		}
	}
	else if (data_moves[move].type == 18) {
		if (player) {
			originalpokemon[player] = currentSave.party[currentPlayer];
			currentSave.party[currentPlayer] = enemyParty[currentEnemy];
			if (currentSave.party[currentPlayer].id < 80) {
				zx7_Decompress(playerSprite, PKMNSD2[currentSave.party[currentPlayer].id]);
			}
			else {
				zx7_Decompress(playerSprite, PKMNSD3[currentSave.party[currentPlayer].id - 80]);
			}
		}
		else {
			originalpokemon[player] = enemyParty[currentEnemy];
			enemyParty[currentEnemy] = currentSave.party[currentPlayer];
			if (enemyParty[currentEnemy].id < 80) {
				zx7_Decompress(enemySprite, PKMNSD0[enemyParty[currentEnemy].id]);
			}
			else {
				zx7_Decompress(enemySprite, PKMNSD1[enemyParty[currentEnemy].id - 80]);
			}
		}
	}
	/* After storing energy for two turns, use bide */
	else if (data_moves[move].type == 20) {
		TakeDamage(!player, bideDamage[player] * 2);
		attackTurns[player] = 0;
	}
	else if (data_moves[move].type == 21) {
		if (player) {
			data_pokemon[currentSave.party[currentPlayer].id].element1 = data_pokemon[enemyParty[currentEnemy].id].element1;
		}
		else {
			data_pokemon[enemyParty[currentEnemy].id].element1 = data_pokemon[currentSave.party[currentPlayer].id].element1;
		}
		converted[player] = true;
	}
	else if (data_moves[move].type == 22) {
		TakeDamage(player, stats[player].health / 4);
		substituteHealth[player] += stats[player].health / 4;
		sprintf(str, "%s created a substitute to take damage", username);
		text_Display(str);
	}
	/* Apply Stat or Status Effects */
	if (data_moves[move].type == 4 || (rand() % 100) < data_moves[move].statuschance) {
		if (data_moves[move].statustype == 30) {
			e = rand() % 3;
		}
		else {
			e = 3;
		}
		if (data_moves[move].statustype < 7) {
			if (mistTurns[!player] || statMods[!player][data_moves[move].statustype] == 0) {
				if (data_moves[move].type == 4) {
					text_Display("It Failed");
				}
			}
			else {
				statMods[!player][data_moves[move].statustype] --;
				sprintf(str, "%s's %s fell", nonusername, text_status2[data_moves[move].statustype]);
				text_Display(str);
			}
		}
		if (data_moves[move].statustype == 7) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 1;
				sprintf(str, "%s was poisoned", nonusername);
				text_Display(str);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed");
			}
		}
		if (data_moves[move].statustype == 8 || e == 0) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 2;
				sprintf(str, "%s was burned", nonusername);
				text_Display(str);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed");
			}
		}
		if (data_moves[move].statustype == 9 || e == 1) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 3;
				sprintf(str, "%s was paralysed", nonusername);
				text_Display(str);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed");
			}
		}
		if (data_moves[move].statustype == 10) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 4;
				sprintf(str, "%s fell asleep", nonusername);
				text_Display(str);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed");
			}
		}
		if (data_moves[move].statustype == 11 || e == 2) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 5;
				sprintf(str, "%s was frozen", nonusername);
				text_Display(str);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed");
			}
		}

		if (data_moves[move].statustype == 12) {
			confusedTurns[!player] = (rand() % 4) + 1;
			sprintf(str, "%s was confused", nonusername);
			text_Display(str);
		}
		if (data_moves[move].statustype == 13) {
			leech[!player] = true;
			sprintf(str, "%s was leeched", nonusername);
			text_Display(str);
		}
		if (data_moves[move].statustype == 14) {
			trapTurns[!player] = (rand() % 2) + 4;
			sprintf(str, "%s was trapped", nonusername);
			text_Display(str);
		}
		if (data_moves[move].statustype == 15) {
			reflectTurns[player] = 5;
		}
		if (data_moves[move].statustype == 16) {
			lightScreenTurns[player] = 5;
		}
		if (data_moves[move].statustype == 17) {
			mistTurns[player] = 5;
		}
		if (data_moves[move].statustype == 18 || data_moves[move].statustype == 38) {
			flinch[!player] = true;
		}
		if (data_moves[move].statustype == 27) {
			highCritRatio[player] = true;
		}
		if (data_moves[move].statustype == 28) {
			statMods[player][0]++;
			statMods[player][1]++;
		}
		if (data_moves[move].statustype == 29) {
			rage[player] = true;
		}
		if (data_moves[move].statustype == 35 && player) {
			payday += currentSave.party[currentPlayer].level*2;
		}
	}
	if (attackTurns[player] == 1 && data_moves[move].statustype == 32) {
		attackTurns[player] = 0;
		confusedTurns[player] = (rand() % 4) + 1;
		sprintf(str, "%s became confused", username);
		text_Display(str);
	}
	RedrawCharacters(0);
}

void ApplyStatus(bool player) {
	char name[20];
	if (player) {
		strcpy(name, playerName);
	}
	else {
		strcpy(name, enemyName);
	}
	/* Poison */
	if (*currentStatusPointer[player] == 1) {
		sprintf(str, "%s was hurt by its poison", name);
		text_Display(str);
		TakeDamage(player, stats[player].health / 12);
	}
	/* Burn */
	if (*currentStatusPointer[player] == 2) {
		sprintf(str, "%s was hurt by its burn", name);
		text_Display(str);
		TakeDamage(player, stats[player].health / 12);
	}
	/* Leech */
	if (leech[player]) {
		sprintf(str, "%s's health was stolen by leech seed", name);
		text_Display(str);
		TakeDamage(player, stats[player].health / 8);
		Heal(!player, stats[player].health / 8);
	}
	/* Trap */
	if (trapTurns[player] > 0) {
		sprintf(str, "%s's was hurt by the trap", name);
		text_Display(str);
		TakeDamage(player, stats[player].health / 16);
		trapTurns[player]--;
	}

	reflectTurns[player]--;
	lightScreenTurns[player]--;
	mistTurns[player]--;
	if (disabledTurns[player] == 1) {
		disabledMove[player] = 0;
	}
	disabledTurns[player]--;
}
/* When a new pokemon is sent out, reset everything 
   This also decompresses the new pokemon's sprite
 */
void ResetStatus(bool player) {
	i = 0;
	while (i < sizeof(statMods[player])) {
		statMods[player][i] = 6;
		i++;
	}
	confusedTurns[player] = 0;
	leech[player] = false;
	leech[!player] = false;
	trapTurns[player] = 0;
	reflectTurns[!player] = 0;
	lightScreenTurns[!player] = 0;
	mistTurns[player] = 0;
	flinch[player] = false;
	flinch[!player] = false;
	highCritRatio[player] = false;
	rage[player] = false;
	disabledMove[player] = 0;
	disabledTurns[player] = 0;
	lastMove[player] = 0;
	attackTurns[player] = 0;
	air[player] = false;
	chosenMove[player] = 0;
	transformed[player] = false;
	converted[player] = false;
	substituteHealth[player] = 0;

	if (player) {
		uint8_t pokemonID;
		sprintf(playerName, "%s", data_pokemon[currentSave.party[currentPlayer].id].name);
		stats[1] = stats_CalculateStats(currentSave.party[currentPlayer]);

		pokemonID = currentSave.party[currentPlayer].id;
		if (pokemonID < 80) {
			zx7_Decompress(playerSprite, PKMNSD2[pokemonID]);
		}
		else {
			zx7_Decompress(playerSprite, PKMNSD3[pokemonID - 80]);
		}
		currentStatusPointer[1] = &currentSave.party[currentPlayer].currentstatus;
	}
	else {
		uint8_t pokemonID;
		sprintf(enemyName, "Enemy %s", data_pokemon[enemyParty[currentEnemy].id].name);
		stats[0] = stats_CalculateStats(enemyParty[currentEnemy]);

		pokemonID = enemyParty[currentEnemy].id;
		
		if (pokemonID < 80) {
			zx7_Decompress(enemySprite, PKMNSD0[pokemonID]);
		}
		else {
			zx7_Decompress(enemySprite, PKMNSD1[pokemonID - 80]);
		}
		currentStatusPointer[0] = &enemyParty[currentEnemy].currentstatus;
	}
}

void AnimateDamage(bool player) {
	RedrawCharacters(player + 1); Wait(5);
	RedrawCharacters(0); Wait(5);
	RedrawCharacters(player + 1); Wait(5);
	RedrawCharacters(0); Wait(5);
	RedrawCharacters(player + 1); Wait(5);
	RedrawCharacters(0); Wait(5);
}
void DrawHealthBar(void) {
	int healthRatio;

	gfx_PrintStringXY(data_pokemon[enemyParty[currentEnemy].id].name, 17, 29);
	sprintf(str, "Lv%u", enemyParty[currentEnemy].level);
	gfx_PrintStringXY(str, 98, 29);
	gfx_Sprite(hpBarSprite, 40, 40);
	healthRatio = (enemyParty[currentEnemy].currenthealth * 78) / stats[0].health;
	if (healthRatio > 34) {
		gfx_SetColor(colors[8]);
	}
	else if (healthRatio > 14) {
		gfx_SetColor(colors[9]);
	}
	else {
		gfx_SetColor(colors[10]);
	}
	gfx_FillRectangle(58, 42, healthRatio, 6);
	if (enemyParty[currentEnemy].currentstatus > 0) {
		gfx_TransparentSprite_NoClip(statusIcons[enemyParty[currentEnemy].currentstatus - 1], 18, 41);
	}

	gfx_PrintStringXY(data_pokemon[currentSave.party[currentPlayer].id].name, 181, 129);
	sprintf(str, "Lv%u", currentSave.party[currentPlayer].level);
	gfx_PrintStringXY(str, 264, 129);
	gfx_Sprite(hpBarSprite, 204, 140);
	healthRatio = (currentSave.party[currentPlayer].currenthealth * 78) / stats[1].health;
	if (healthRatio > 34) {
		gfx_SetColor(colors[8]);
	}
	else if (healthRatio > 14) {
		gfx_SetColor(colors[9]);
	}
	else {
		gfx_SetColor(colors[10]);
	}
	gfx_FillRectangle(222, 142, healthRatio, 6);
}
void TakeDamage(bool player, int amount) {
	bideDamage[player] += amount;
	if (substituteHealth[player] > 0) {
		text_Display("The substitute blocked the damage");
		if (substituteHealth[player] <= amount) {
			substituteHealth[player] = 0;
			text_Display("The substitute was destroyed");
		}
		else {
			substituteHealth[player] -= amount;
		}
	}
	else {
		int oldHealth, newHealth, healthIndex;
		if (player) {
			oldHealth = currentSave.party[currentPlayer].currenthealth;
			newHealth = currentSave.party[currentPlayer].currenthealth;
			if (currentSave.party[currentPlayer].currenthealth <= amount) {
				currentSave.party[currentPlayer].currenthealth = 0;
				newHealth = 0;
			}
			else {
				newHealth -= amount;
			}
			RedrawCharacters(3);
			for (healthIndex = 1; healthIndex <= 16; healthIndex++) {
				currentSave.party[currentPlayer].currenthealth = oldHealth - (((oldHealth - newHealth)*healthIndex) / 16);
				DrawHealthBar();
				gfx_SwapDraw();
			}
		}
		else {
			oldHealth = enemyParty[currentEnemy].currenthealth;
			newHealth = enemyParty[currentEnemy].currenthealth;
			if (enemyParty[currentEnemy].currenthealth <= amount) {
				enemyParty[currentEnemy].currenthealth = 0;
				newHealth = 0;
			}
			else {
				newHealth -= amount;
			}
			RedrawCharacters(3);
			for (healthIndex = 1; healthIndex <= 16; healthIndex++) {
				enemyParty[currentEnemy].currenthealth = oldHealth - (((oldHealth - newHealth)*healthIndex) / 16);
				DrawHealthBar();
				gfx_SwapDraw();
			}
		}
	}
	RedrawCharacters(0);
}
void Heal(bool player, int amount) {
	int oldHealth, newHealth, healthIndex;
	if (player) {
		oldHealth = currentSave.party[currentPlayer].currenthealth;
		newHealth = currentSave.party[currentPlayer].currenthealth;
		newHealth += amount;
		if (newHealth >= stats[1].health) {
			newHealth = stats[1].health;
		}
		RedrawCharacters(3);
		for (healthIndex = 1; healthIndex <= 16; healthIndex++) {
			currentSave.party[currentPlayer].currenthealth = oldHealth - (((oldHealth - newHealth)*healthIndex) / 16);
			DrawHealthBar();
			gfx_SwapDraw();
		}
	}
	else {
		oldHealth = enemyParty[currentEnemy].currenthealth;
		newHealth = enemyParty[currentEnemy].currenthealth;
		newHealth += amount;
		if (newHealth >= stats[0].health) {
			newHealth = stats[0].health;
		}
		RedrawCharacters(3);
		for (healthIndex = 1; healthIndex <= 16; healthIndex++) {
			enemyParty[currentEnemy].currenthealth = oldHealth - (((oldHealth - newHealth)*healthIndex) / 16);
			DrawHealthBar();
			gfx_SwapDraw();
		}
	}
	RedrawCharacters(0);
}



void AddXp(void) {
	uint16_t xpgain = (enemyParty[currentEnemy].level * data_pokemon[enemyParty[currentEnemy].id].xpdrop) / 7;
	currentSave.party[currentPlayer].xp += xpgain;
	while ((currentSave.party[currentPlayer].level < 100) && (currentSave.party[currentPlayer].xp >= xpPerLevel[data_pokemon[currentSave.party[currentPlayer].id].xptype][currentSave.party[currentPlayer].level + 1])) {
		currentSave.party[currentPlayer].level++;
		sprintf(str, "%s leveled up", data_pokemon[currentSave.party[currentPlayer].id].name);
		text_Display(str);
		for (i = 0; i < 10; i++)
		{
			if (data_pokemon[currentSave.party[currentPlayer].id].movelevels[i] == currentSave.party[currentPlayer].level) {
				sprintf(str, "%s wants to learn %s", data_pokemon[currentSave.party[currentPlayer].id].name, data_moves[data_pokemon[currentSave.party[currentPlayer].id].moveids[i]].name);
				text_Display(str);
				sprintf(str, "Choose a move to replace with %s", data_moves[data_pokemon[currentSave.party[currentPlayer].id].moveids[i]].name);
				text_Display(str);
				switch (text_AskQuestion4(data_moves[currentSave.party[currentPlayer].moves[0]].name, data_moves[currentSave.party[currentPlayer].moves[1]].name, data_moves[currentSave.party[currentPlayer].moves[2]].name, data_moves[currentSave.party[currentPlayer].moves[3]].name))
				{
				case 1:
					currentSave.party[currentPlayer].moves[0] = data_pokemon[currentSave.party[currentPlayer].id].moveids[i];
					break;
				case 2:
					currentSave.party[currentPlayer].moves[1] = data_pokemon[currentSave.party[currentPlayer].id].moveids[i];
					break;
				case 3:
					currentSave.party[currentPlayer].moves[2] = data_pokemon[currentSave.party[currentPlayer].id].moveids[i];
					break;
				case 4:
					currentSave.party[currentPlayer].moves[3] = data_pokemon[currentSave.party[currentPlayer].id].moveids[i];
					break;
				}
			}
			i++;
		}
		if (currentSave.party[currentPlayer].level == data_pokemon[currentSave.party[currentPlayer].id].evolvelevel) {
			sprintf(str, "%s is evolving", data_pokemon[currentSave.party[currentPlayer].id].name);
			text_Display(str);
			if (text_AskQuestion2("Continue", "Stop") == 1) {
				currentSave.party[currentPlayer].id = data_pokemon[currentSave.party[currentPlayer].id].evolveid;
			}
		}
	}
	currentSave.party[currentPlayer].healthEV += data_pokemon[enemyParty[currentEnemy].id].basehealth;
	currentSave.party[currentPlayer].attackEV += data_pokemon[enemyParty[currentEnemy].id].baseattack;
	currentSave.party[currentPlayer].defenceEV += data_pokemon[enemyParty[currentEnemy].id].basedefence;
	currentSave.party[currentPlayer].specialattackEV += data_pokemon[enemyParty[currentEnemy].id].basespecialattack;
	currentSave.party[currentPlayer].specialdefenceEV += data_pokemon[enemyParty[currentEnemy].id].basespecialdefence;
	currentSave.party[currentPlayer].speedEV += data_pokemon[enemyParty[currentEnemy].id].basespeed;
	if (currentSave.party[currentPlayer].healthEV >= 65200) {
		currentSave.party[currentPlayer].healthEV = 65200;
	}
	if (currentSave.party[currentPlayer].attackEV >= 65200) {
		currentSave.party[currentPlayer].attackEV = 65200;
	}
	if (currentSave.party[currentPlayer].defenceEV >= 65200) {
		currentSave.party[currentPlayer].defenceEV = 65200;
	}
	if (currentSave.party[currentPlayer].specialattackEV >= 65200) {
		currentSave.party[currentPlayer].specialattackEV = 65200;
	}
	if (currentSave.party[currentPlayer].specialdefenceEV >= 65200) {
		currentSave.party[currentPlayer].specialdefenceEV = 65200;
	}
	if (currentSave.party[currentPlayer].speedEV >= 65200) {
		currentSave.party[currentPlayer].speedEV = 65200;
	}
}

bool Capture(uint8_t ball) {
	uint8_t n1;
	uint8_t n2;
	uint32_t f;
	bool Captured = false;
	uint8_t slot = 0;
	/* Check if pokemon is Captured*/
	if (!wild) {
		text_Display("You cannot catch trainer\npokemon.");
		return false;
	}
	if (ball == 20) {
		Captured = true;
	}
	else {
		f = stats[false].health * 3;
		f -= 2 * enemyParty[currentEnemy].currenthealth;
		f *= data_pokemon[enemyParty[currentEnemy].id].catchrate;

		if (ball == 1) {
			f *= 3;
			f /= 2;
		}
		if (ball == 2) {
			f *= 2;
		}
		if (ball == 21 && ft) {
			f *= 5;
			f /= 2;
		}
		
		if (ball == 22) {
			f *= 5;
			f /= 2;
		}
		if (ball == 23 && currentSave.party[currentPlayer].level > enemyParty[currentEnemy].level) {
			f *= 5;
			f /= 2;
		}
		if (ball == 25 && (data_pokemon[enemyParty[currentEnemy].id].element1 == 3 || data_pokemon[enemyParty[currentEnemy].id].element2 == 3)) {
			f *= 5;
			f /= 2;
		}

		f /= stats[false].health * 3;
		if (*currentStatusPointer[false] == 4 || *currentStatusPointer[false] == 5) {
			f += 10;
		}
		if (*currentStatusPointer[false] == 1 || *currentStatusPointer[false] == 2 || *currentStatusPointer[false] == 3) {
			f += 20;
		}

		if (f >= (rand() % 255)) {
			Captured = true;
			if (ball == 24) {
				enemyParty[currentEnemy].currenthealth = stats[0].health;
			}
		}
	}
	/* Find open slot if pokemon is Captured */
	if (Captured) {
		sprintf(str, "%s was caught.", data_pokemon[enemyParty[currentEnemy].id].name);
		text_Display(str);
		for (slot = 0; slot < 36; slot++) {
			if (currentSave.party[slot].id == 0) {
				currentSave.party[slot] = enemyParty[currentEnemy];
				slot = 40;
			}
			i++;
		}
		if (slot == 36) {
			text_Display("No open slots for pokemon.");
		}
	}
	else {
		sprintf(str, "%s broke free.", data_pokemon[enemyParty[currentEnemy].id].name);
		text_Display(str);
	}
	return Captured;
}

