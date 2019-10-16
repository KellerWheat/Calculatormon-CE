#include <graphx.h>
#include <stdio.h>
#include <stdint.h>
#include <keypadc.h>

#include "items.h"
#include "text.h"
#include "savegame.h"
#include "data.h"
#include "misc.h"
#include "stats.h"
#include "map.h"
#include "menu.h"

void HealHp(uint8_t index, uint16_t amount);
void HealStatus(uint8_t index, uint8_t type);

const uint16_t healAmounts[] = { 20,50,120,1000 };

/* Item Data */

char itemNames[20][16] = {
	"Poke Ball",
	"Great Ball",
	"Ultra Ball",
	"Master Ball",
	"Potion",
	"Super Potion",
	"Hyper Potion",
	"Max Potion",
	"Full Heal",
	"Full Restore",
	"Antidote",
	"Burn Heal",
	"Parlyz Heal",
	"Ice Heal",
	"Awakening",
	"Thunder Stone",
	"Moon Stone",
	"Fire Stone",
	"Leaf Stone",
	"Water Stone"
};
char itemDescriptions[20][68] = {
	"A tool for catching wild Pokemon",
	"A good Ball with a higher catch rate than a Poke Ball",
	"A better Ball with a higher catch rate than a Great Ball",
	"The best Ball that catches a Pokemon without fail",
	"Restores the HP of a Pokemon by 20 points",
	"Restores the HP of a Pokemon by 50 points",
	"Restores the HP of a Pokemon by 120 points",
	"Fully restores the HP of a Pokemon",
	"Heals all the status problems of one Pokemon",
	"Fully restores the HP and heals any status problems of one Pokemon",
	"Heals a poisoned Pokemon",
	"Heals a burned Pokemon",
	"Heals a paralyzed Pokemon",
	"Heals a frozen Pokemon",
	"Awakens a sleeping Pokemon",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
};

/* Used in shop */
uint16_t itemPrices[20] = {
	200,600,1200,15000,300,700,1200,2500,600,3000,100,250,200,250,200,2100,2100,2100,2100,2100
};
uint8_t itemCount = 20;


void items_IndexToName(char *name, uint8_t index) {
	if (index < 20) {
		sprintf(name, itemNames[index]);
	}
	else {
		sprintf(name, data_moves[index-19].name);
	}
}

bool items_UseItem(uint8_t index) {
	char itemName[20];
	if (index < 4 || currentSave.playerItems[index] == 0) {
		return false;
	}
	i = menu_PokemonMenu(false);
	if (i == 0 || currentSave.party[i - 1].id == 0 || currentSave.party[i - 1].currenthealth == 0) {
		return false;
	}
	i -= 1;
	if (index < 8) {
		currentSave.playerItems[index]--;
		HealHp(i, healAmounts[index - 4]);
	}
	else if (index < 10) {
		currentSave.playerItems[index]--;
		HealStatus(i, 0);
		HealStatus(i, 1);
		HealStatus(i, 2);
		HealStatus(i, 3);
		HealStatus(i, 4);
		if (index == 9) {
			HealHp(i, 1000);
		}
	}
	else if (index < 15) {
		currentSave.playerItems[index]--;
		HealStatus(i, index-10);
	}
	else if (index < 20) {
		sprintf(str, "%s has evolved", data_pokemon[currentSave.party[i].id].name);
		if (data_pokemon[currentSave.party[i].id].evolvelevel == index + 86) {
			currentSave.party[i].id = data_pokemon[currentSave.party[i].id].evolveid;
		}
		else if (data_pokemon[currentSave.party[i].id].evolvelevel == 106) {
			if (index = 19) {
				currentSave.party[i].id += 1;
			}
			else if (index = 17) {
				currentSave.party[i].id += 2;
			}
			else if (index = 15) {
				currentSave.party[i].id += 3;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
		currentSave.playerItems[index]--;
		text_Display(str, false);
		return true;
	}

	if (index >= 20) {
		if (data_pokemon[currentSave.party[i].id].element1 == data_moves[index - 19].element || data_pokemon[currentSave.party[i].id].element2 == data_moves[index - 19].element) {
			sprintf(str, "Choose a move to replace with %s", data_moves[index - 19].name);
			text_Display(str, false);
			switch (text_AskQuestion4(data_moves[currentSave.party[i].moves[0]].name, data_moves[currentSave.party[i].moves[1]].name, data_moves[currentSave.party[i].moves[2]].name, data_moves[currentSave.party[i].moves[3]].name, false))
			{
			case 1:
				currentSave.party[i].moves[0] = index - 19;
				currentSave.party[i].pp[0] = data_moves[currentSave.party[i].moves[0]].uses;
				break;
			case 2:
				currentSave.party[i].moves[1] = index - 19;
				currentSave.party[i].pp[1] = data_moves[currentSave.party[i].moves[1]].uses;
				break;
			case 3:
				currentSave.party[i].moves[2] = index - 19;
				currentSave.party[i].pp[2] = data_moves[currentSave.party[i].moves[2]].uses;
				break;
			case 4:
				currentSave.party[i].moves[3] = index - 19;
				currentSave.party[i].pp[3] = data_moves[currentSave.party[i].moves[3]].uses;
				break;
			}
		}
		else {
			sprintf(str, "%s cannot learn %s", data_pokemon[currentSave.party[i].id].name, data_moves[index - 19].name);
			text_Display(str, false);
			return false;
		}
	}


	items_IndexToName(&itemName, index);
	sprintf(str, "Used a %s on %s", itemName, data_pokemon[currentSave.party[i].id].name);
	text_Display(str, true);
	return true;
}

void HealHp(uint8_t index, uint16_t amount) {
	currentSave.party[index].currenthealth += amount;
	if (currentSave.party[index].currenthealth > stats_CalculateStats(currentSave.party[index]).health) {
		currentSave.party[index].currenthealth = stats_CalculateStats(currentSave.party[index]).health;
	}
}
void HealStatus(uint8_t index, uint8_t type) {
	currentSave.party[index].currentstatus = 0;
}
