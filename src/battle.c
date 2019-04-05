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
#include "gfx/PKMNSD4.h"
#include "gfx/PKMNSD5.h"
#include "gfx/PKMNSD6.h"
#include "gfx/PKMNSD7.h"
#include "gfx/PKMNSD8.h"
#include "gfx/PKMNSD9.h"
#include "gfx/PKMNSD10.h"
#include "gfx/PKMNSD11.h"

bool playerturn(void);
bool enemyturn(void);
void redrawcharacters(void);
void attack(bool player, uint8_t move);

uint8_t * getstatus(bool player);
void applystatus(bool player);
void resetstatus(bool player);
void loadsprite(bool player);
void takedamage(bool player, int amount);
void heal(bool player, int amount);

void winfight(void);
void losefight(void);
void addxp(void);
bool capture(uint8_t ball);


const int statmodvalues[] = { 25,29,33,40,50,67,100,150,200,250,300,350,400 };
const struct pokemonData clearcharacter = { 0 };

uint8_t currentplayer;
uint8_t currentenemy;

char playername[20];
char enemyname[20];

struct pokemonData enemyparty[6];
uint8_t attackturn = 0;
uint8_t chosenmove[2];
bool wild;

struct pokemonStats stats[2];
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

uint8_t disabledmove[2];
uint8_t disabledturns[2];
uint8_t lastmove[2];
uint8_t attackturns[2];
bool air[2];
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
	PKMNSD4_init();
	PKMNSD5_init();
	PKMNSD6_init();
	PKMNSD7_init();
	PKMNSD8_init();
	PKMNSD9_init();
	PKMNSD10_init();
	PKMNSD11_init();
}
void battle_Setup(void) {
	int pokemonIndex;
	for (pokemonIndex = 0; pokemonIndex < 6; pokemonIndex++) {//This makes sure the pokemon sent out has not fainted
		if (party[pokemonIndex].currenthealth != 0 && party[pokemonIndex].id != 0) {
			currentplayer = pokemonIndex;
			pokemonIndex = 6;
		}
	}
	gfx_SetPalette(battle_gfx_pal, sizeof_battle_gfx_pal, 0);
	SetColors(1);
	gfx_SetTextScale(1, 1);
	gfx_SetTextFGColor(colors[1]);
	battleMenuState1 = 0;
	battleMenuState2 = 0;
	battleMenuCurrent = 1;

	stats[0] = stats_CalculateStats(enemyparty[0]);
	stats[1] = stats_CalculateStats(party[0]);
	currentenemy = 0;
	attackturn = 0;

	resetstatus(true);
	resetstatus(false);

	run = false;
}
int battle_Loop(void) {
	/* Battle */
	redrawcharacters();
	/* Switch to a new poketmon or lose battle if health is 0 */
	if (party[currentplayer].currenthealth == 0) {
		sprintf(str, "%s fainted", playername);
		text_Display(str, false);
		attackturn = 0;

		i = 0;
		while (i < 6) {
			if (party[i].currenthealth != 0 && party[i].id != 0) {
				currentplayer = i;
				resetstatus(true);
				return 1;
			}
			i++;
		}

		losefight();
		return 0;
	}
	if (enemyparty[currentenemy].currenthealth == 0) {
		sprintf(str, "%s fainted", enemyname);
		text_Display(str, false);
		addxp();
		attackturn = 0;

		i = 0;
		while (i < 6) {
			if (enemyparty[i].currenthealth != 0 && enemyparty[i].id != 0) {
				currentenemy = i;
				resetstatus(false);
				return 1;
			}
			i++;
		}


		winfight();
		return 0;
	}
	/* If 0 determine who attacks first, if 1 or 2 let other character attack, if 3 do end of turn stuff */
	if (attackturn == 0) {
		while (!playerturn()) {};
		while (!enemyturn()) {};
		if (run) {
			return 0;
		}

		if ((stats[0].speed / (1+(getstatus(0)[2]*3))) > (stats[1].speed / (1 + (getstatus(1)[2] * 3)))) {
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
			i = text_AskQuestion6(data_pokemon[party[0].id].name, data_pokemon[party[1].id].name, data_pokemon[party[2].id].name, data_pokemon[party[3].id].name, data_pokemon[party[4].id].name, data_pokemon[party[5].id].name);
			if (i != 0 && (i - 1) != currentplayer && party[i - 1].id != 0) {
				currentplayer = i - 1;
				resetstatus(true);
				return true;
			}
			return false;
		case 4:
			if (!wild) {
				text_Display("Can't run from trainers!", true);
				return false;
			}
			//NOT REAL SPEED FUNCTION
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
		selectedMove = text_AskQuestion4(data_moves[party[currentplayer].move1].name, data_moves[party[currentplayer].move2].name, data_moves[party[currentplayer].move3].name, data_moves[party[currentplayer].move4].name, true);
		if (selectedMove == 0) {
			battleMenuState1 = 0;
			return false;
		}
		switch (selectedMove)
		{
		case 1:
			if (party[currentplayer].move1 == 0 || disabledmove[true] == 1) {
				return false;
			}
			battleMenuState1 = 0;
			chosenmove[true] = party[currentplayer].move1;
			break;
		case 2:
			if (party[currentplayer].move2 == 0 || disabledmove[true] == 2) {
				return false;
			}
			battleMenuState1 = 0;
			chosenmove[true] = party[currentplayer].move2;
			break;
		case 3:
			if (party[currentplayer].move3 == 0 || disabledmove[true] == 3) {
				return false;
			}
			battleMenuState1 = 0;
			chosenmove[true] = party[currentplayer].move3;
			break;
		case 4:
			if (party[currentplayer].move4 == 0 || disabledmove[true] == 4) {
				return false;
			}
			battleMenuState1 = 0;
			chosenmove[true] = party[currentplayer].move4;
			break;
		}
	}
	/* Items */
	else if (battleMenuState1 == 2) {
		battleMenuCurrent = 0;
		battleMenuState2 = 0;
	
	redrawitems:
		gfx_SetDrawScreen();
		gfx_SetColor(colors[0]);
		gfx_FillRectangle(20, 20, 280, 200);
		gfx_SetColor(colors[1]);
		gfx_Rectangle(20, 20, 280, 200);
		gfx_SetColor(colors[0]);

		gfx_PrintStringXY(">", 25, 25 + 20 * (battleMenuCurrent));
		for (i = 0; i < 10 && i + battleMenuState2 < 16; i++) {
			gfx_PrintStringXY(itemNames[i + battleMenuState2], 35, 25 + i * 20);
			sprintf(str, "%u", playerItems[i + battleMenuState2]);
			gfx_PrintStringXY(str, 150, 25 + i * 20);
		}
		Wait(20);

		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		kb_Scan();
		while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
			kb_Scan();
			if ((kb_Data[7] & kb_Down) && battleMenuCurrent + battleMenuState2 < 15) {
				battleMenuCurrent++;
				if (battleMenuCurrent == 5 && battleMenuState2 < 5) {
					battleMenuState2++;
					battleMenuCurrent--;
				}
				goto redrawitems;
			}
			if ((kb_Data[7] & kb_Up) && battleMenuCurrent != 0) {
				battleMenuCurrent--;
				if (battleMenuCurrent == 4 && battleMenuState2 > 0) {
					battleMenuState2--;
					battleMenuCurrent++;
				}
				goto redrawitems;
			}
		}
		gfx_SetDrawBuffer();
		redrawcharacters();
		if (kb_Data[6] & kb_Clear) {
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
			battleMenuState1 = 0;
			return false;
		}
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }

		if (playerItems[battleMenuCurrent] == 0) {
			battleMenuCurrent = 0;
			return false;
		}
		battleMenuState1 = 0;
		if (battleMenuCurrent < 4) {
			sprintf(str, "Used a %s", itemNames[battleMenuCurrent]);
			text_Display(str, true);
			playerItems[battleMenuCurrent]--;
			if (capture(battleMenuCurrent + 1)) {
				winfight();
				run = true;
				return true;
			}
		}
		else {
			if (items_UseItem(battleMenuCurrent)) {
				redrawcharacters();
			}
			else {
				return false;
			}
		}
	}
	return true;
}
bool enemyturn() {
	if (attackturns[false] > 0) {
		chosenmove[false] = lastmove[false];
		return true;
	}
	switch (rand() % 4)
	{
	case 0:
		if (enemyparty[currentenemy].move1 == 0 || disabledmove[false] == 1) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].move1;
		break;
	case 1:
		if (enemyparty[currentenemy].move2 == 0 || disabledmove[false] == 2) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].move2;
		break;
	case 2:
		if (enemyparty[currentenemy].move3 == 0 || disabledmove[false] == 3) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].move3;
		break;
	case 3:
		if (enemyparty[currentenemy].move4 == 0 || disabledmove[false] == 4) {
			return false;
		}
		chosenmove[false] = enemyparty[currentenemy].move4;
		break;
	}
	return true;
}
void redrawcharacters(void) {
	int pokemonID, statusIndex;
	gfx_FillScreen(colors[0]);
	gfx_SetColor(colors[1]);

	pokemonID = party[currentplayer].id;
	if (pokemonID < 28) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD6[i], 20, 60, 2, 2);
	}
	else if (pokemonID < 56) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD7[pokemonID - 28], 20, 60, 2, 2);
	}
	else if (pokemonID < 84) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD8[pokemonID - 56], 20, 60, 2, 2);
	}
	else if (pokemonID < 112) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD9[pokemonID - 84], 20, 60, 2, 2);
	}
	else if (pokemonID < 140) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD10[pokemonID - 112], 20, 60, 2, 2);
	}
	else if (pokemonID < 160) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD11[pokemonID - 140], 20, 60, 2, 2);
	}

	pokemonID = enemyparty[currentenemy].id;
	if (pokemonID < 30) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD0[i], 190, 10, 2, 2);
	}
	else if (pokemonID < 57) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD1[pokemonID - 30], 190, 10, 2, 2);
	}
	else if (pokemonID < 84) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD2[pokemonID - 57], 190, 10, 2, 2);
	}
	else if (pokemonID < 109) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD3[pokemonID - 84], 190, 10, 2, 2);
	}
	else if (pokemonID < 132) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD4[pokemonID - 109], 190, 10, 2, 2);
	}
	else if (pokemonID < 160) {
		gfx_ScaledSprite_NoClip((gfx_sprite_t*)PKMNSD5[pokemonID - 132], 190, 10, 2, 2);
	}
	

	/* Display Information about player and enemy */
	gfx_Rectangle(20, 20, 150, 30);
	sprintf(str, "LV%u %s", enemyparty[currentenemy].level, data_pokemon[enemyparty[currentenemy].id].name);
	gfx_PrintStringXY(str, 25, 25);
	sprintf(str, "%u/%u", enemyparty[currentenemy].currenthealth, stats[0].health);
	gfx_PrintStringXY(str, 25, 35);

	gfx_Rectangle(140, 125, 150, 30);
	sprintf(str, "LV%u %s", party[currentplayer].level, data_pokemon[party[currentplayer].id].name);
	gfx_PrintStringXY(str, 145, 130);
	sprintf(str, "%u/%u", party[currentplayer].currenthealth, stats[1].health);
	gfx_PrintStringXY(str, 145, 140);

	for (statusIndex = 0; statusIndex < 5; statusIndex++) {
		gfx_SetColor(colors[statuscolors[statusIndex]]);
		if (getstatus(1)[statusIndex]) {
			gfx_FillCircle(215 + (15* statusIndex), 145, 4);
		}
		if(getstatus(0)[statusIndex]) {
			gfx_FillCircle(95 + (15* statusIndex), 25, 4);
		}
	}

	gfx_SwapDraw();
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
	sprintf(str, "%s used %s", username, data_moves[move].name);
	text_Display(str, true);

	if (data_moves[move].type == 16) {
		move = 1 + (rand() % 165);
	}
	if (data_moves[move].type == 17) {
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
	if (data_moves[move].statustype == 33) {
		if (attackturns[player] == 0) {
			attackturns[player] = 1;
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
	

	/* Exit if character's status prevents movement */
	if (getstatus(player)[2] && (rand() % 4 == 1)) {
		sprintf(str, "%s is paralysed", username);
		text_Display(str, true);
		return;
	}
	if (getstatus(player)[3]) {
		sprintf(str, "%s is asleep", username);
		text_Display(str, true);
		return;
	}
	if (getstatus(player)[4]) {
		sprintf(str, "%s is frozen", username);
		text_Display(str, true);
		return;
	}
	if (confusedturns[player] > 0) {
		confusedturns[player] --;
		if (rand() % 2 == 0) {
			sprintf(str, "%s hurt itself in its confusion", username);
			text_Display(str, true);
			return;
		}
	}

	if (air[!player] || (data_moves[move].accuracy * statmodvalues[statmods[player][5]] / statmodvalues[statmods[!player][6]]) < (rand() % 100)) {
		text_Display("It Missed", true);
		if (data_moves[move].statustype == 25) {
			takedamage(player, stats[player].health);
		}
		return;
	}
	if (getstatus(!player)[3] == false && data_moves[move].statustype == 26) {
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
		if (data_moves[move].statustype == 24 || highcritratio[player]) {
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
		getstatus(player)[3] = 2;
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
				text_Display("It Failed", true);
			}
			else {
				statmods[!player][data_moves[move].statustype] --;
				sprintf(str, "%s's %s fell", true, nonusername, text_status2[data_moves[move].statustype]);
				text_Display(str, true);
			}
		}
		if (data_moves[move].statustype == 7) {
			getstatus(!player)[0] = true;
			sprintf(str, "%s was poisoned", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 8 || e == 0) {
			getstatus(!player)[1] = true;
			sprintf(str, "%s was burned", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 9 || e == 1) {
			getstatus(!player)[2] = true;
			sprintf(str, "%s was paralysed", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 10) {
			getstatus(!player)[3] = true;
			sprintf(str, "%s fell asleep", nonusername);
			text_Display(str, true);
		}
		if (data_moves[move].statustype == 11 || e == 2) {
			getstatus(!player)[4] = true;
			sprintf(str, "%s was frozen", nonusername);
			text_Display(str, true);
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
		if (data_moves[move].statustype == 18) {
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
	}

	

	if (attackturns[player] == 1 && data_moves[move].statustype == 32) {
		attackturns[player] = 0;
		confusedturns[player] = (rand() % 4) + 1;
		sprintf(str, "%s became confused", username);
		text_Display(str, true);
	}
}

uint8_t * getstatus(bool player) {
	if (player) {
		return(party[currentplayer].currentstatus);
	}
	else {
		return(enemyparty[currentenemy].currentstatus);
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
	if (getstatus(player)[0]) {
		sprintf(str, "%s was hurt by its poison", name);
		text_Display(str, true);
		takedamage(player, stats[player].health / 12);
	}
	/* Burn */
	if (getstatus(player)[1]) {
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
/* When a new pokemon is sent out, reset everything */
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

	if (player) {
		sprintf(playername, "%s", data_pokemon[party[currentplayer].id].name);
		stats[1] = stats_CalculateStats(party[currentplayer]);
	}
	else {
		sprintf(enemyname, "Enemy %s", data_pokemon[enemyparty[currentenemy].id].name);\
		stats[0] = stats_CalculateStats(enemyparty[currentenemy]);
	}
	loadsprite(player);
}

void loadsprite(bool player) {
	/*ti_var_t file = ti_Open("appvar0_DATA", "r");
	if (player) {
		ti_Read(&playersprite.data, sizeof(playersprite.data), 1, file);
	}
	else {
		ti_Read(&enemysprite.data, sizeof(enemysprite.data), 1, file);
	}
	ti_CloseAll();*/
}

void takedamage(bool player, int amount) {
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



void winfight(void) {
	char str1[16];
	if (!wild) {
		playerMoney += enemyparty[0].level * 40;
		if (indoors) {
			defeatedTrainersIndoors[currentBuilding][currentTrainer] = true;
			if (data_buildingZoneData[currentBuilding].trainerreward[currentTrainer] != 0) {
				if (data_buildingZoneData[currentBuilding].trainerreward[currentTrainer] == 255) {
					badgeCount++;
					sprintf(str, "Recieved a badge");
				}
				else {
					playerItems[data_buildingZoneData[currentBuilding].trainerreward[currentTrainer] - 1]++;
					items_IndexToName(str1, data_buildingZoneData[currentBuilding].trainerreward[currentTrainer] - 1);
					if (data_buildingZoneData[currentBuilding].trainerreward[currentTrainer] - 1 >= 20) {
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
			if (data_zoneData[currentZone].trainerreward[currentTrainer] != 0) {
				playerItems[data_zoneData[currentZone].trainerreward[currentTrainer]-1]++;
				items_IndexToName(str1, data_zoneData[currentZone].trainerreward[currentTrainer] - 1);
				if (data_zoneData[currentZone].trainerreward[currentTrainer] - 1 >= 20) {
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
void losefight(void) {
	text_Display("All your pokemon have fainted", false);
	map_Respawn();
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
				sprintf(str, "%s wants to learn%s", data_pokemon[party[currentplayer].id].name, data_moves[data_pokemon[party[currentplayer].id].moveids[i]].name);
				text_Display(str, false);
				sprintf(str, "Choose a move to replace with %s", data_moves[data_pokemon[party[currentplayer].id].moveids[i]].name);
				text_Display(str, false);
				switch (text_AskQuestion4(data_moves[party[currentplayer].move1].name, data_moves[party[currentplayer].move2].name, data_moves[party[currentplayer].move3].name, data_moves[party[currentplayer].move4].name, false))
				{
				case 1:
					party[currentplayer].move1 = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				case 2:
					party[currentplayer].move2 = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				case 3:
					party[currentplayer].move3 = data_pokemon[party[currentplayer].id].moveids[i];
					break;
				case 4:
					party[currentplayer].move4 = data_pokemon[party[currentplayer].id].moveids[i];
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
		if (getstatus(false)[3] || getstatus(false)[4]) {
			if (n1 < 25) {
				captured = true;
			}
			else {
				n2 -= 25;
			}
		}
		if (getstatus(false)[0] || getstatus(false)[1] || getstatus(false)[2]) {
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
			if (party[i].id == 0) {
				party[i] = enemyparty[currentenemy];
				i = 40;
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

