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
#include "gfx/PKMNSD5.h"
#include "gfx/PKMNSD6.h"

void SetupBattleGfx(void);

bool playerturn(void);
bool enemyturn(void);
int selectMove(void);
void redrawcharacters(void);
void attack(bool player, uint8_t move);

uint8_t CalculateXpPercent(void);
void applystatus(bool player);
void resetstatus(bool player);
void takedamage(bool player, int amount);
void heal(bool player, int amount);
bool capture(uint8_t ball);
void addxp(void);


const int statmodvalues[] = { 25,29,33,40,50,67,100,150,200,250,300,350,400 };
const struct pokemonData clearcharacter = { 0 };

uint8_t currentplayer;
uint8_t currentenemy;

char playername[20];
char enemyname[20];

gfx_sprite_t *playerSprite;
gfx_sprite_t *enemySprite;

gfx_sprite_t *backgroundSprite;
gfx_sprite_t *hpBarSprite;
gfx_sprite_t *statusSprites[5];

struct pokemonData enemyparty[6];
uint8_t attackturn = 0;
uint8_t chosenmove[2];
bool wild;

struct pokemonStats stats[2];
uint8_t *currentStatusPointer[2];

uint8_t statmods[2][7];
uint8_t confusedturns[2];
bool leech[2];
uint8_t trapturns[2];
uint8_t reflectturns[2];
uint8_t lightscreenturns[2];
uint8_t mistturns[2];
bool flinch[2];
bool highcritratio[2];
bool rage[2];
bool transformed[2];
bool converted[2];
struct pokemonData originalpokemon[2];
uint16_t bidedamage[2];
uint16_t substitutehealth[2];

uint8_t disabledmove[2];
uint8_t disabledturns[2];
uint8_t lastmove[2];
uint8_t attackturns[2];
bool air[2];
uint16_t payday;
bool run;

uint8_t battleMenuState1 = 0;
uint8_t battleMenuState2 = 0;
uint8_t battleMenuCurrent = 1;

const uint8_t statuscolors[5] = { 2,3,4,5,6 };

void battle_Initialize(void) {
	PKMNSD0_init();
	PKMNSD1_init();
	PKMNSD2_init();
	PKMNSD3_init();
	PKMNSD5_init();
	PKMNSD6_init();
}
void battle_Setup(void) {
	int pokemonIndex;
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {//This makes sure the pokemon sent out has not fainted
		if (party[pokemonIndex].currenthealth != 0 && party[pokemonIndex].id != 0) {
			currentplayer = pokemonIndex;
			pokemonIndex = 6;
		}
	}
	SetupBattleGfx();

	battleMenuState1 = 0;
	battleMenuState2 = 0;
	battleMenuCurrent = 1;

	
	stats[0] = stats_CalculateStats(enemyparty[0]);
	stats[1] = stats_CalculateStats(party[0]);
	currentenemy = 0;
	attackturn = 0;

	resetstatus(true);
	resetstatus(false);
	payday = 0;

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

	if (party[currentplayer].id < 80) {
		zx7_Decompress(playerSprite, PKMNSD2[party[currentplayer].id]);
	}
	else {
		zx7_Decompress(playerSprite, PKMNSD3[party[currentplayer].id - 80]);
	}
	if (enemyparty[currentenemy].id < 80) {
		zx7_Decompress(enemySprite, PKMNSD0[enemyparty[currentenemy].id]);
	}
	else {
		zx7_Decompress(enemySprite, PKMNSD1[enemyparty[currentenemy].id - 80]);
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
	redrawcharacters();
	/* Switch to a new poketmon or lose battle if health is 0 */
	if (party[currentplayer].currenthealth == 0) {
		sprintf(str, "%s fainted", playername);
		text_Display(str, false);
		attackturn = 0;

		if (transformed[true]) {
			party[currentplayer] = originalpokemon[true];
		}
		if (converted[true]) {
			data_pokemon[party[currentplayer].id].element1 = 1;
		}

		i = 0;
		while (i < 6) {
			if (party[i].currenthealth != 0 && party[i].id != 0) {
				currentplayer = i;
				resetstatus(true);
				return 1;
			}
			i++;
		}

		map_LoseFight();
		return 0;
	}
	if (enemyparty[currentenemy].currenthealth == 0) {
		sprintf(str, "%s fainted", enemyname);
		text_Display(str, false);
		addxp();
		attackturn = 0;

		if (transformed[false]) {
			enemyparty[currentenemy] = originalpokemon[false];
		}
		if (converted[false]) {
			data_pokemon[enemyparty[currentenemy].id].element1 = 1;
		}

		i = 0;
		while (i < 6) {
			if (enemyparty[i].currenthealth != 0 && enemyparty[i].id != 0) {
				currentenemy = i;
				resetstatus(false);
				return 1;
			}
			i++;
		}

		playerMoney += payday;
		map_WinFight(wild, enemyparty[0].level * 40);
		return 0;
	}
	/* If 0 determine who attacks first, if 1 or 2 let other character attack, if 3 do end of turn stuff */
	if (attackturn == 0) {
		while (!playerturn()) {};
		while (!enemyturn()) {};
		if (run) {
			return 0;
		}
		/* Paralysis gets a speed reduction */
		if ((((stats[0].speed / (1+(((*currentStatusPointer[0]) == 4) * 3))) > (stats[1].speed / (1 + (((*currentStatusPointer[1]) == 4) * 3)))) || data_moves[chosenmove[false]].statustype == 31) && data_moves[chosenmove[true]].statustype != 31) {
			attack(false, chosenmove[false]);
			attackturn = 1;
		}
		else {
			attack(true, chosenmove[true]);
			
			attackturn = 2;
		}
	}
	else if(attackturn < 3) {
		if (attackturn == 1) {
			attack(true, chosenmove[true]);
			attackturn = 3;
		}
		if (attackturn == 2) {
			attack(false, chosenmove[false]);
			attackturn = 3;
		}
	}
	else {
		/* After both turns */
		applystatus(true);
		applystatus(false);
		flinch[true] = false;
		flinch[false] = false;
		attackturn = 0;
		return 1;
	}
	

	return 1;
}


void battle_SpawnWild(uint8_t id, uint8_t minlevel, uint8_t maxlevel) {
	int partyIndex;
	for(partyIndex = 0; partyIndex < 6; partyIndex++){
		enemyparty[partyIndex] = clearcharacter;
	}
	enemyparty[0] = stats_NewCharacter(id,(rand() % (maxlevel-minlevel+1))+minlevel);
	wild = true;
}
void battle_SpawnTrainer(uint8_t ids[6][16], uint8_t levels[6][16], uint8_t trainernum) {
	int partyIndex;
	for(partyIndex = 0; partyIndex < 6; partyIndex++){
		enemyparty[partyIndex] = stats_NewCharacter(ids[partyIndex][trainernum],levels[partyIndex][trainernum]);
	}
	wild = false;
}

bool playerturn() {
	if (attackturns[true] > 0) {
		chosenmove[true] = lastmove[true];
		return true;
	}
	i = 0;
	/* Select Whether to fight, run, ect */
	if (battleMenuState1 == 0) {
		switch (text_AskQuestion4("Fight","Item","Switch","Run", true))
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
			if (i != 0 && (i - 1) != currentplayer && party[i - 1].id != 0) {
				currentplayer = i - 1;
				resetstatus(true);
				redrawcharacters();
				return true;
			}
			redrawcharacters();
			return false;
		case 4:
			if (!wild) {
				text_Display("Can't run from trainers!", true);
				return false;
			}
			//NOT REAL RUN FUNCTION
			if (stats[1].speed > stats[0].speed || (rand() % 3) == 0) {
				run = true;
				return true;
			}
			else {
				text_Display("Failed to run away!", true);
				return true;
			}
			return false;
		}
	}
	/* Select which move to use */
	if (battleMenuState1 == 1) {
		int selectedMove;
		selectedMove = selectMove();
		battleMenuState1 = 0;
		/* Checks if the move can be used */
		if (selectedMove == 0 || party[currentplayer].pp[selectedMove - 1] == 0 || disabledmove[true] == selectedMove) {
			return false;
		}
		chosenmove[true] = party[currentplayer].moves[selectedMove - 1];
		party[currentplayer].pp[selectedMove - 1]--;
	}
	/* Items */
	else if (battleMenuState1 == 2) {
		int chosenItem;
		battleMenuState1 = 0;
		battle_End();
		chosenItem = menu_Items(true);
		SetupBattleGfx();
		if (chosenItem == -1) {
			return false;
		}
		if (chosenItem < 4) {
			redrawcharacters();
			sprintf(str, "Used a %s", itemNames[chosenItem]);
			text_Display(str, true);
			playerItems[chosenItem]--;
			chosenmove[true] = 0;
			if (capture(chosenItem + 1)) {
				map_WinFight(wild, enemyparty[0].level * 40);
				run = true;
				return true;
			}
		}
		else {
			battle_End();
			if (items_UseItem(chosenItem)) {
				chosenmove[true] = 0;
				SetupBattleGfx();
				redrawcharacters();
			}
			else {
				SetupBattleGfx();
				redrawcharacters();
				return false;
			}
		}
	}
	return true;
}

int selectMove(void) {
	int8_t tv1, tv2, iconIndex;
	tv1 = 0;
	tv2 = 0;

	textBoxType = 2;
	gfx_Blit(gfx_screen);
	text_DrawTextBox();



	gfx_PrintStringXY(">", 15, 190);
	gfx_PrintStringXY(data_moves[party[currentplayer].moves[0]].name, 25, 190);
	gfx_PrintStringXY(data_moves[party[currentplayer].moves[1]].name, 25, 205);
	gfx_PrintStringXY(data_moves[party[currentplayer].moves[2]].name, 145, 190);
	gfx_PrintStringXY(data_moves[party[currentplayer].moves[3]].name, 145, 205);

	if (party[currentplayer].moves[tv1 + 2 * tv2] != 0) {
		gfx_Sprite(typeIcons[data_moves[party[currentplayer].moves[tv1 + 2 * tv2]].element - 1], 271, 193);
		sprintf(str, "%u/%u", party[currentplayer].pp[tv1 + 2 * tv2], data_moves[party[currentplayer].moves[tv1 + 2 * tv2]].uses);
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
			if (party[currentplayer].moves[tv1 + 2 * tv2] != 0) {
				gfx_Sprite(typeIcons[data_moves[party[currentplayer].moves[tv1 + 2 * tv2]].element - 1], 271, 193);
				sprintf(str, "%u/%u", party[currentplayer].pp[tv1 + 2 * tv2], data_moves[party[currentplayer].moves[tv1 + 2 * tv2]].uses);
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
			if (party[currentplayer].moves[tv1 + 2 * tv2] != 0) {
				gfx_Sprite(typeIcons[data_moves[party[currentplayer].moves[tv1 + 2 * tv2]].element - 1], 271, 193);
				sprintf(str, "%u/%u", party[currentplayer].pp[tv1 + 2 * tv2], data_moves[party[currentplayer].moves[tv1 + 2 * tv2]].uses);
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

bool enemyturn() {
	if (attackturns[false] > 0) {
		chosenmove[false] = lastmove[false];
		return true;
	}
	switch (rand() % 4)
	{
	case 0:
		if (enemyparty[currentenemy].moves[0] == 0 || disabledmove[false] == 1) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].moves[0];
		break;
	case 1:
		if (enemyparty[currentenemy].moves[1] == 0 || disabledmove[false] == 2) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].moves[1];
		break;
	case 2:
		if (enemyparty[currentenemy].moves[2] == 0 || disabledmove[false] == 3) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].moves[2];
		break;
	case 3:
		if (enemyparty[currentenemy].moves[3] == 0 || disabledmove[false] == 4) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].moves[3];
		break;
	}
	return true;
}
void redrawcharacters(void) {
	int statusIndex, healthRatio;
	gfx_SetColor(colors[1]);
	text_DrawTextBox();

	gfx_ScaledSprite_NoClip(backgroundSprite, 0, 0, 2, 2);
	gfx_ScaledTransparentSprite_NoClip(playerSprite, 20, 64, 2, 2);
	gfx_ScaledTransparentSprite_NoClip(enemySprite, 190, 8, 2, 2);

	/* Display Information about player and enemy */
	
	gfx_PrintStringXY(data_pokemon[enemyparty[currentenemy].id].name, 17, 29);
	sprintf(str, "Lv%u", enemyparty[currentenemy].level);
	gfx_PrintStringXY(str, 98, 29);
	gfx_Sprite(hpBarSprite, 40, 40);
	healthRatio = (enemyparty[currentenemy].currenthealth * 78) / stats[0].health;
	if (healthRatio > 34) {
		gfx_SetColor(colors[8]);
	}else if (healthRatio > 14) {
		gfx_SetColor(colors[9]);
	}else {
		gfx_SetColor(colors[10]);
	}
	gfx_FillRectangle(58, 42, healthRatio, 6);
	if (enemyparty[currentenemy].currentstatus > 0) {
		gfx_TransparentSprite_NoClip(statusIcons[enemyparty[currentenemy].currentstatus - 1], 18, 41);
	}

	gfx_PrintStringXY(data_pokemon[party[currentplayer].id].name, 181, 129);
	sprintf(str, "Lv%u", party[currentplayer].level);
	gfx_PrintStringXY(str, 264, 129);
	gfx_Sprite(hpBarSprite, 204, 140);
	healthRatio = (party[currentplayer].currenthealth * 78) / stats[1].health;
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
	
	sprintf(str, "%u/%u", party[currentplayer].currenthealth, stats[1].health);
	gfx_PrintStringXY(str, 247, 153);

	gfx_SetTextFGColor(colors[11]);
	gfx_PrintStringXY("XP", 193, 166);
	gfx_SetColor(colors[7]);
	if (party[currentplayer].level < 100) {
		gfx_FillRectangle(210, 168, CalculateXpPercent(), 4);
	}
	if (party[currentplayer].currentstatus > 0) {
		gfx_TransparentSprite_NoClip(statusIcons[party[currentplayer].currentstatus - 1], 182, 141);
	}

	gfx_SetColor(colors[0]);
	gfx_SetTextFGColor(colors[1]);
	gfx_SwapDraw();
}

uint8_t CalculateXpPercent(void) {
	int cXp, sXp, mXp;
	cXp = party[currentplayer].xp;
	sXp = xpPerLevel[data_pokemon[party[currentplayer].id].xptype][party[currentplayer].level];
	mXp = xpPerLevel[data_pokemon[party[currentplayer].id].xptype][party[currentplayer].level + 1];
	return((uint8_t)((92 * (cXp - sXp)) / (mXp - sXp)));

}

void battle_End(void) {
	free(playerSprite);
	free(enemySprite);
	free(backgroundSprite);
	free(hpBarSprite);
	FreeIcons();
}

void attack(bool player, uint8_t move) {
	
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
		strcpy(username, playername);
		strcpy(nonusername, enemyname);
		userlevel = party[currentplayer].level;
		nonuserlevel = enemyparty[currentenemy].level;
		userelement1 = data_pokemon[party[currentplayer].id].element1;
		userelement2 = data_pokemon[party[currentplayer].id].element2;
		nonuserelement1 = data_pokemon[enemyparty[currentenemy].id].element1;
		nonuserelement2 = data_pokemon[enemyparty[currentenemy].id].element2;

	}
	else {
		strcpy(username, enemyname);
		strcpy(nonusername, playername);
		userlevel = enemyparty[currentenemy].level;
		nonuserlevel = party[currentplayer].level;
		userelement1 = data_pokemon[enemyparty[currentenemy].id].element1;
		userelement2 = data_pokemon[enemyparty[currentenemy].id].element2;
		nonuserelement1 = data_pokemon[party[currentplayer].id].element1;
		nonuserelement2 = data_pokemon[party[currentplayer].id].element2;
	}

	if (flinch[player]) {
		sprintf(str, "%s flinched", username);
		text_Display(str, true);
	}
	if (data_moves[move].type == 16) {
		sprintf(str, "%s used %s", username, data_moves[move].name);
		text_Display(str, true);
		move = 1 + (rand() % 165);
	}


	
	if (data_moves[move].type == 17) {
		sprintf(str, "%s used %s", username, data_moves[move].name);
		text_Display(str, true);
		move = lastmove[!player];
	}
	lastmove[player] = move;

	if (data_moves[move].type == 19 && data_moves[lastmove[!player]].type != 0) {
		text_Display("It Failed", true);
		return;
	}

	if (data_moves[move].statustype == 32) {
		if (attackturns[player] == 0) {
			attackturns[player] = (rand() % 2) + 2;
		}
		attackturns[player]--;
	}
	if (data_moves[move].statustype == 33 || data_moves[move].statustype == 36 || data_moves[move].statustype == 37 || data_moves[move].statustype == 38) {
		if (attackturns[player] == 0) {
			attackturns[player] = 1;
			sprintf(str, "%s is building up power", username, data_moves[move].name);
			text_Display(str, true);
			return;
		}
		else if (attackturns[player] == 1) {
			attackturns[player] = 0;
		}
	}
	if (data_moves[move].statustype == 34) {
		if (attackturns[player] == 0) {
			attackturns[player] = 1;
			air[player] = true;
			return;
		}
		else if (attackturns[player] == 1) {
			attackturns[player] = 0;
			air[player] = false;
		}
	}
	if (data_moves[move].type == 20 && attackturns[player] != 1) {
		if (attackturns[player] == 0) {
			bidedamage[player] = 0;
			attackturns[player] = 2;
			sprintf(str, "%s used Bide", username);
			text_Display(str, true);
		}
		else {
			attackturns[player]--;
		}
		sprintf(str, "%s is storing energy", username);
		text_Display(str, true);
		return;
	}

	/* Exit if character's status prevents movement */
	if ((*currentStatusPointer[player] == 3) && (rand() % 4 == 1)) {
		sprintf(str, "%s is paralysed", username);
		text_Display(str, true);
		return;
	}
	if (*currentStatusPointer[player] == 4) {
		if (rand() % 3 == 0) {
			*currentStatusPointer[player] = 0;
			sprintf(str, "%s woke up", username);
			text_Display(str, true);
		}
		else {
			sprintf(str, "%s is asleep", username);
			text_Display(str, true);
			return;
		}
	}
	if (*currentStatusPointer[player] == 5) {
		if (rand() % 5 == 0) {
			*currentStatusPointer[player] = 0;
			sprintf(str, "%s thawed out", username);
			text_Display(str, true);
		}
		else {
			sprintf(str, "%s is frozen", username);
			text_Display(str, true);
			return;
		}
	}
	if (confusedturns[player] > 0) {
		confusedturns[player] --;
		if (rand() % 2 == 0) {
			sprintf(str, "%s hurt itself in its confusion", username);
			text_Display(str, true);
			return;
		}
	}

	sprintf(str, "%s used %s", username, data_moves[move].name);
	text_Display(str, true);

	if (air[!player] || (data_moves[move].accuracy * statmodvalues[statmods[player][5]] / statmodvalues[statmods[!player][6]]) < (rand() % 100)) {
		text_Display("It Missed", true);
		if (data_moves[move].statustype == 25) {
			takedamage(player, stats[player].health);
		}
		return;
	}
	if (!(*currentStatusPointer[player] == 4) && data_moves[move].statustype == 26) {
		text_Display("It Failed", true);
	}
	if (data_moves[move].statustype == 22) {
		hitcount = (rand() % 3) + 1;
	}
	if (data_moves[move].statustype == 23) {
		hitcount = 1;
	}

	


startattack:
	if (data_moves[move].type < 2 || data_moves[move].type == 19) {
		if (data_moves[move].type == 0) {
			damage = ((((2 * userlevel / 5) + 2)*data_moves[move].power*(stats[player].attack*statmodvalues[statmods[player][0]] / 100) / (stats[!player].defence * statmodvalues[statmods[!player][1]] / 100)) / 50) + 2;
		}
		else if (data_moves[move].type == 1) {
			damage = ((((2 * userlevel / 5) + 2)*data_moves[move].power*(stats[player].specialattack*statmodvalues[statmods[player][2]] / 100) / (stats[!player].specialdefence * statmodvalues[statmods[!player][3]] / 100)) / 50) + 2;
		}
		else {
			damage = ((((2 * userlevel / 5) + 2) * data_moves[lastmove[!player]].power * 2 * (stats[player].attack*statmodvalues[statmods[player][0]] / 100) / (stats[!player].defence * statmodvalues[statmods[!player][1]] / 100)) / 50) + 2;
		}
		damage *= ((rand() % 16) + 85);//  /100
		damage *= elements[data_moves[move].element][nonuserelement1];// /2
		damage *= elements[data_moves[move].element][nonuserelement2];// /2
		damage /= 400;
		if (data_moves[move].element == userelement1 || data_moves[move].element == userelement2) {
			damage *= 1.5;
		}

		/* Critical Hit */
		if (data_moves[move].statustype == 24 || highcritratio[player] || data_moves[move].statustype == 36) {
			if (player) {
				if (data_pokemon[party[currentplayer].id].basespeed * 4 < (rand() % 256)) {
					damage *= (2 * userlevel + 5) / (userlevel + 5);
				}
			}
			else {
				if (data_pokemon[enemyparty[currentenemy].id].basespeed * 4 < (rand() % 256)) {
					damage *= (2 * userlevel + 5) / (userlevel + 5);
				}
			}
			text_Display("Critical Hit", true);
		}
		else {
			if (player) {
				if (data_pokemon[party[currentplayer].id].basespeed / 2 < (rand() % 256)) {
					damage *= (2 * userlevel + 5) / (userlevel + 5);
				}
			}
			else {
				if (data_pokemon[enemyparty[currentenemy].id].basespeed / 2 < (rand() % 256)) {
					damage *= (2 * userlevel + 5) / (userlevel + 5);
				}
			}
		}

		takedamage(!player, damage);

		if (data_moves[move].statustype == 21 || data_moves[move].statustype == 26) {
			heal(player, damage / 2);
		}
		if (data_moves[move].statustype == 19) {
			takedamage(player, damage / 3);
		}
		if (hitcount > 0) {
			hitcount--;
			redrawcharacters();
			Wait(15);
			goto startattack;
		}

		if (elements[data_moves[move].element][nonuserelement1] * elements[data_moves[move].element][nonuserelement2] < 4) {
			text_Display("Not Very Effective", true);
		}
		if (elements[data_moves[move].element][nonuserelement1] * elements[data_moves[move].element][nonuserelement2] > 4) {
			text_Display("Super Effective", true);
		}
		if (rage[!player]) {
			statmods[!player][0]++;
		}
		redrawcharacters();
	}
	/* Raise user stat */
	else if (data_moves[move].type == 2) {
		if (statmods[player][data_moves[move].statustype] + data_moves[move].power > 12) {
			statmods[player][data_moves[move].statustype] = 12;
		}
		else {
			statmods[player][data_moves[move].statustype] += data_moves[move].power;
		}
		sprintf(str, "%s's %s rose", username, text_status2[data_moves[move].statustype]);
		text_Display(str, true);
	}
	/* Lower non-user stat */
	else if (data_moves[move].type == 3) {
		if (statmods[!player][data_moves[move].statustype] < data_moves[move].power) {
			statmods[!player][data_moves[move].statustype] = 0;
		}
		else {
			statmods[!player][data_moves[move].statustype] -= data_moves[move].power;
		}
		sprintf(str, "%s's %s fell", nonusername, text_status2[data_moves[move].statustype]);
		text_Display(str, true);
	}
	else if (data_moves[move].type == 5) {
		takedamage(!player, data_moves[move].power);
	}
	else if (data_moves[move].type == 6 || data_moves[move].type == 7) {
		if (wild) {
			run = true;
		}
		else {
			text_Display("It Failed", true);
		}
	}
	else if (data_moves[move].type == 8) {
		heal(player, stats[player].health / 2);
	}
	else if (data_moves[move].type == 9) {
		takedamage(!player, userlevel);
	}
	else if (data_moves[move].type == 10) {
		takedamage(!player, userlevel * 20 / (10 + (rand() % 20)));
	}
	else if (data_moves[move].type == 11) {
		takedamage(!player, stats[!player].health / 2);
	}
	else if (data_moves[move].type == 12) {
		i = 0;
		while (i < sizeof(statmods[player])) {
			statmods[player][i] = 6;
			i++;
		}
		while (i < sizeof(statmods[!player])) {
			statmods[player][i] = 6;
			i++;
		}
	}
	else if (data_moves[move].type == 13) {
		*currentStatusPointer[player] = 4;
		heal(player, 1000);
	}
	else if (data_moves[move].type == 14) {
		disabledturns[!player] = (rand() % 7) + 2;
		disabledmove[!player] = lastmove[!player];
	}
	else if (data_moves[move].type == 15) {
		if ((int)userlevel - (int)nonuserlevel + 30 > (rand() % 100)) {
			takedamage(!player, 800);
		}
		else {
			text_Display("It Missed", true);
		}
	}
	else if (data_moves[move].type == 18) {
		if (player) {
			originalpokemon[player] = party[currentplayer];
			party[currentplayer] = enemyparty[currentenemy];
		}
		else {
			originalpokemon[player] = enemyparty[currentenemy];
			enemyparty[currentenemy] = party[currentplayer];
		}
	}
	else if (data_moves[move].type == 20) {
		takedamage(!player, bidedamage[player] * 2);
		attackturns[player] = 0;
	}
	else if (data_moves[move].type == 21) {
		if (player) {
			data_pokemon[party[currentplayer].id].element1 = data_pokemon[enemyparty[currentenemy].id].element1;
		}
		else {
			data_pokemon[enemyparty[currentenemy].id].element1 = data_pokemon[party[currentplayer].id].element1;
		}
		converted[player] = true;
	}
	else if (data_moves[move].type == 22) {
		takedamage(player, stats[player].health / 4);
		substitutehealth[player] += stats[player].health / 4;
		sprintf(str, "%s created a substitute to take damage", username);
		text_Display(str, true);
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
			if (mistturns[!player] || statmods[!player][data_moves[move].statustype] == 0) {
				if (data_moves[move].type == 4) {
					text_Display("It Failed", true);
				}
			}
			else {
				statmods[!player][data_moves[move].statustype] --;
				sprintf(str, "%s's %s fell", nonusername, text_status2[data_moves[move].statustype]);
				text_Display(str, true);
			}
		}
		if (data_moves[move].statustype == 7) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 1;
				sprintf(str, "%s was poisoned", nonusername);
				text_Display(str, true);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed", true);
			}
		}
		if (data_moves[move].statustype == 8 || e == 0) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 2;
				sprintf(str, "%s was burned", nonusername);
				text_Display(str, true);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed", true);
			}
		}
		if (data_moves[move].statustype == 9 || e == 1) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 3;
				sprintf(str, "%s was paralysed", nonusername);
				text_Display(str, true);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed", true);
			}
		}
		if (data_moves[move].statustype == 10) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 4;
				sprintf(str, "%s fell asleep", nonusername);
				text_Display(str, true);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed", true);
			}
		}
		if (data_moves[move].statustype == 11 || e == 2) {
			if (*currentStatusPointer[!player] == 0) {
				*currentStatusPointer[!player] = 5;
				sprintf(str, "%s was frozen", nonusername);
				text_Display(str, true);
			}
			else if (data_moves[move].type == 4) {
				text_Display("It failed", true);
			}
		}

		if (data_moves[move].statustype == 12) {
			confusedturns[!player] = (rand() % 4) + 1;
			sprintf(str, "%s was confused", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 13) {
			leech[!player] = true;
			sprintf(str, "%s was leeched", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 14) {
			trapturns[!player] = (rand() % 2) + 4;
			sprintf(str, "%s was trapped", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 15) {
			reflectturns[player] = 5;
		}
		if (data_moves[move].statustype == 16) {
			lightscreenturns[player] = 5;
		}
		if (data_moves[move].statustype == 17) {
			mistturns[player] = 5;
		}
		if (data_moves[move].statustype == 18 || data_moves[move].statustype == 38) {
			flinch[!player] = true;
		}
		if (data_moves[move].statustype == 27) {
			highcritratio[player] = true;
		}
		if (data_moves[move].statustype == 28) {
			statmods[player][0]++;
			statmods[player][1]++;
		}
		if (data_moves[move].statustype == 29) {
			rage[player] = true;
		}
		if (data_moves[move].statustype == 35 && player) {
			payday += party[currentplayer].level*2;
		}
	}

	

	if (attackturns[player] == 1 && data_moves[move].statustype == 32) {
		attackturns[player] = 0;
		confusedturns[player] = (rand() % 4) + 1;
		sprintf(str, "%s became confused", username);
		text_Display(str, true);
	}
}

void applystatus(bool player) {
	char name[20];
	if (player) {
		strcpy(name, playername);
	}
	else {
		strcpy(name, enemyname);
	}
	/* Poison */
	if (*currentStatusPointer[player] == 1) {
		sprintf(str, "%s was hurt by its poison", name);
		text_Display(str, true);
		takedamage(player, stats[player].health / 12);
	}
	/* Burn */
	if (*currentStatusPointer[player] == 2) {
		sprintf(str, "%s was hurt by its burn", name);
		text_Display(str, true);
		takedamage(player, stats[player].health / 12);
	}
	/* Leech */
	if (leech[player]) {
		sprintf(str, "%s's health was stolen by leech seed", name);
		text_Display(str, true);
		takedamage(player, stats[player].health / 8);
		heal(!player, stats[player].health / 8);
	}
	/* Trap */
	if (trapturns[player] > 0) {
		sprintf(str, "%s's was hurt by the trap", name);
		text_Display(str, true);
		takedamage(player, stats[player].health / 16);
		trapturns[player]--;
	}

	reflectturns[player]--;
	lightscreenturns[player]--;
	mistturns[player]--;
	if (disabledturns[player] == 1) {
		disabledmove[player] = 0;
	}
	disabledturns[player]--;
}
/* When a new pokemon is sent out, reset everything 
   This also decompresses the new pokemon's sprite
 */
void resetstatus(bool player) {
	i = 0;
	while (i < sizeof(statmods[player])) {
		statmods[player][i] = 6;
		i++;
	}
	confusedturns[player] = 0;
	leech[player] = false;
	leech[!player] = false;
	trapturns[player] = 0;
	reflectturns[!player] = 0;
	lightscreenturns[!player] = 0;
	mistturns[player] = 0;
	flinch[player] = false;
	flinch[!player] = false;
	highcritratio[player] = false;
	rage[player] = false;
	disabledmove[player] = 0;
	disabledturns[player] = 0;
	lastmove[player] = 0;
	attackturns[player] = 0;
	air[player] = false;
	chosenmove[player] = 0;
	transformed[player] = false;
	converted[player] = false;
	substitutehealth[player] = 0;

	if (player) {
		uint8_t pokemonID;
		sprintf(playername, "%s", data_pokemon[party[currentplayer].id].name);
		stats[1] = stats_CalculateStats(party[currentplayer]);

		pokemonID = party[currentplayer].id;
		if (pokemonID < 80) {
			zx7_Decompress(playerSprite, PKMNSD2[pokemonID]);
		}
		else {
			zx7_Decompress(playerSprite, PKMNSD3[pokemonID - 80]);
		}
		currentStatusPointer[1] = &party[currentplayer].currentstatus;
	}
	else {
		uint8_t pokemonID;
		sprintf(enemyname, "Enemy %s", data_pokemon[enemyparty[currentenemy].id].name);
		stats[0] = stats_CalculateStats(enemyparty[currentenemy]);

		pokemonID = enemyparty[currentenemy].id;
		
		if (pokemonID < 80) {
			zx7_Decompress(enemySprite, PKMNSD0[pokemonID]);
		}
		else {
			zx7_Decompress(enemySprite, PKMNSD1[pokemonID - 80]);
		}
		currentStatusPointer[0] = &enemyparty[currentenemy].currentstatus;
	}
}

void takedamage(bool player, int amount) {
	bidedamage[player] += amount;
	if (substitutehealth[player] > 0) {
		text_Display("The substitute blocked the damage", true);
		if (substitutehealth[player] <= amount) {
			substitutehealth[player] = 0;
			text_Display("The substitute was destroyed", true);
		}
		else {
			substitutehealth[player] -= amount;
		}
	}
	else {
		if (player) {
			if (party[currentplayer].currenthealth <= amount) {
				party[currentplayer].currenthealth = 0;
			}
			else {
				party[currentplayer].currenthealth -= amount;
			}
		}
		else {
			if (enemyparty[currentenemy].currenthealth <= amount) {
				enemyparty[currentenemy].currenthealth = 0;
			}
			else {
				enemyparty[currentenemy].currenthealth -= amount;
			}
		}
	}
	redrawcharacters();
}
void heal(bool player, int amount) {
	if (player) {
		party[currentplayer].currenthealth += amount;
		if (party[currentplayer].currenthealth >= stats[1].health) {
			party[currentplayer].currenthealth = stats[1].health;
		}
	}
	else {
		enemyparty[currentenemy].currenthealth += amount;
		if (enemyparty[currentenemy].currenthealth >= stats[0].health) {
			enemyparty[currentenemy].currenthealth = stats[0].health;
		}
	}
	redrawcharacters();
}



void addxp(void) {
	uint16_t xpgain = (enemyparty[currentenemy].level * data_pokemon[enemyparty[currentenemy].id].xpdrop) / 7;
	party[currentplayer].xp += xpgain;
	while ((party[currentplayer].level < 100) && (party[currentplayer].xp >= xpPerLevel[data_pokemon[party[currentplayer].id].xptype][party[currentplayer].level + 1])) {
		party[currentplayer].level++;
		sprintf(str, "%s leveled up", data_pokemon[party[currentplayer].id].name);
		text_Display(str, false);
		for (i = 0; i < 10; i++)
		{
			if (data_pokemon[party[currentplayer].id].movelevels[i] == party[currentplayer].level) {
				sprintf(str, "%s wants to learn %s", data_pokemon[party[currentplayer].id].name, data_moves[data_pokemon[party[currentplayer].id].moveids[i]].name);
				text_Display(str, false);
				sprintf(str, "Choose a move to replace with %s", data_moves[data_pokemon[party[currentplayer].id].moveids[i]].name);
				text_Display(str, false);
				switch (text_AskQuestion4(data_moves[party[currentplayer].moves[0]].name, data_moves[party[currentplayer].moves[1]].name, data_moves[party[currentplayer].moves[2]].name, data_moves[party[currentplayer].moves[3]].name, false))
				{
				case 1:
					party[currentplayer].moves[0] = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				case 2:
					party[currentplayer].moves[1] = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				case 3:
					party[currentplayer].moves[2] = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				case 4:
					party[currentplayer].moves[3] = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				}
			}
			i++;
		}
		if (party[currentplayer].level == data_pokemon[party[currentplayer].id].evolvelevel) {
			sprintf(str, "%s is evolving", data_pokemon[party[currentplayer].id].name);
			text_Display(str, false);
			if (text_AskQuestion2("Continue", "Stop", false) == 1) {
				party[currentplayer].id = data_pokemon[party[currentplayer].id].evolveid;
			}
		}
	}
	party[currentplayer].healthEV += data_pokemon[enemyparty[currentenemy].id].basehealth;
	party[currentplayer].attackEV += data_pokemon[enemyparty[currentenemy].id].baseattack;
	party[currentplayer].defenceEV += data_pokemon[enemyparty[currentenemy].id].basedefence;
	party[currentplayer].specialattackEV += data_pokemon[enemyparty[currentenemy].id].basespecialattack;
	party[currentplayer].specialdefenceEV += data_pokemon[enemyparty[currentenemy].id].basespecialdefence;
	party[currentplayer].speedEV += data_pokemon[enemyparty[currentenemy].id].basespeed;
}
/* ball is 1-4 */
bool capture(uint8_t ball) {
	uint8_t n1;
	uint8_t n2;
	uint32_t f;
	bool captured = false;
	uint8_t slot = 0;
	/* Check if pokemon is captured*/
	if (!wild) {
		text_Display("You cannot catch trainer pokemon", true);
		return false;
	}
	if (ball == 4) {
		captured = true;
	}
	else {
		if (ball = 1) {
			n1 = rand() % 255;
		}
		if (ball = 2) {
			n1 = rand() % 200;
		}
		if (ball = 3) {
			n1 = rand() % 150;
		}
		n2 = n1;
		if (*currentStatusPointer[false] == 4 || *currentStatusPointer[false] == 5) {
			if (n1 < 25) {
				captured = true;
			}
			else {
				n2 -= 25;
			}
		}
		if (*currentStatusPointer[false] == 1 || *currentStatusPointer[false] == 2 || *currentStatusPointer[false] == 3) {
			if (n1 < 12) {
				captured = true;
			}
			else {
				n2 -= 12;
			}
		}
		if (n2 <= data_pokemon[enemyparty[currentenemy].id].catchrate) {
			f = stats[false].health;
			f *= 255;
			f *= 4;
			f /= enemyparty[currentenemy].currenthealth;
			if (ball == 2) {
				f /= 8;
			}
			else {
				f /= 12;
			}
			if (f >= (rand() % 255)) {
				captured = true;
			}
		}
	}
	/* Find open slot if pokemon is captured */
	if (captured) {
		sprintf(str, "%s was caught", data_pokemon[enemyparty[currentenemy].id].name);
		text_Display(str, false);
		for (slot = 0; slot < 36; slot++) {
			if (party[slot].id == 0) {
				party[slot] = enemyparty[currentenemy];
				slot = 40;
			}
			i++;
		}
		if (slot == 36) {
			text_Display("No open slots for pokemon", false);
		}
	}
	else {
		sprintf(str, "%s broke free", data_pokemon[enemyparty[currentenemy].id].name);
		text_Display(str, true);
	}
	return captured;
}

