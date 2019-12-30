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

const uint16_t healAmounts[] = { 20,50,120,1000, 50,60,80,100 };

/* Item Data */

char itemNames[][16] = {
	"Poke Ball",
	"Great Ball",
	"Ultra Ball",
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
	"Revive",
	"Thunder Stone",
	"Moon Stone",
	"Fire Stone",
	"Leaf Stone",
	"Water Stone",
	"Master Ball",
	"Quick Ball",
	"Net Ball",
	"Level Ball",
	"Heal Ball",
	"Nest Ball",
	"Max Revive",
	"Elixir",
	"Rare Candy",
	"Fresh Water",
	"Soda Pop",
	"Lemonade",
	"Moomoo Milk",
	"Magic Dice",
	"HP Up",
	"Protein",
	"Iron",
	"Calcium",
	"Zinc",
	"Carbos",
};
/* Item Descriptions (Not used here anymore)
	"A tool for catching wild Pokemon",
	"A good Ball with a higher catch rate than a Poke Ball",
	"A better Ball with a higher catch rate than a Great Ball",
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
	"Revives a pokemon with half hp",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"Makes certain species of Pokemon evolve",
	"The best ball that catches any wild Pokemon",
	"A pokeball that works best on the first turn of a battle",
	"A pokeball that works best on Water Pokemon",
	"A pokeball that works best when the enemy is a lower level",
	"A pokeball that heals the caught Pokemon",
	"A pokeball that works best against weaker Pokemon",
	"Revives a Pokemon with full hp",
	"Restores the PP for all of a Pokemon's moves",
	"Raises the Level of a Pokemon",
	"Heals a Pokemon for 50 HP",
	"Heals a Pokemon for 60 HP",
	"Heals a Pokemon for 80 HP",
	"Heals a Pokemon for 100 HP",
	"Changes any Pokemon into a random different Pokemon",
	"Raises a Pokemon's Health",
	"Raises a Pokemon's Attack",
	"Raises a Pokemon's Defence",
	"Raises a Pokemon's Special Attack",
	"Raises a Pokemon's Special Defence",
	"Raises a Pokemon's Speed",
*/

/* Used in shop */
uint16_t itemPrices[15] = {
	200,600,1200,300,700,1200,2500,600,3000,100,250,200,250,200,1500
};
uint8_t itemCount = 15;


void items_IndexToName(char *name, uint8_t index) {
	if (index < 40) {
		sprintf(name, itemNames[index]);
	}
	else {
		sprintf(name, data_moves[index-39].name);
	}
}

bool items_UseItem(uint8_t index) {
	char itemName[20];
	if ((index < 3 || (index >= 20 && index < 26)) && currentSave.playerItems[index] == 0) {
		return false;
	}
	i = menu_PokemonMenu(false);
	if (i == 0 || currentSave.party[i - 1].id == 0 || currentSave.party[i - 1].currenthealth == 0) {
		return false;
	}
	i -= 1;
	if (currentSave.party[i].currenthealth == 0 && index != 14 && index != 26) {
		text_Display("Cannot be used on a fainted\nPokemon");
		return false;
	}
	if (index < 7) {
		currentSave.playerItems[index]--;
		HealHp(i, healAmounts[index - 4]);
	}
	else if (index < 9) {
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
	else if (index < 14) {
		currentSave.playerItems[index]--;
		HealStatus(i, index-9);
	}
	else if (index == 14) {
		currentSave.party[i].currenthealth = stats_CalculateStats(currentSave.party[i]).health/2;
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
			text_Display("Cannot be used on this Pokemon");
			return false;
		}
		currentSave.playerItems[index]--;
		text_Display(str);
		return true;
	}
	else if (index == 26) {
		currentSave.party[i].currenthealth = stats_CalculateStats(currentSave.party[i]).health;
	}
	else if (index == 27) {
		int moveIndex;
		for (moveIndex = 0;  moveIndex < 4;  moveIndex++) {
			currentSave.party[i].pp[moveIndex] = data_moves[currentSave.party[i].moves[moveIndex]].uses;
		}
	}
	else if (index == 28) {
		currentSave.party[i].level += 1;
	}
	else if (index < 33) {
		HealHp(i, healAmounts[index - 25]);
	}
	else if (index == 33) {
		currentSave.party[i].id = rand() % 151;
	}
	else if (index == 34) {
		currentSave.party[i].healthIV += 2;
	}
	else if (index == 35) {
		currentSave.party[i].attackIV += 2;
	}
	else if (index == 36) {
		currentSave.party[i].defenceIV += 2;
	}
	else if (index == 37) {
		currentSave.party[i].specialattackIV += 2;
	}
	else if (index == 38) {
		currentSave.party[i].specialdefenceIV += 2;
	}
	else if (index == 39) {
		currentSave.party[i].speedIV += 2;
	}

	else if (index >= 40) {
		if (data_pokemon[currentSave.party[i].id].element1 == data_moves[index - 39].element || data_pokemon[currentSave.party[i].id].element2 == data_moves[index - 39].element) {
			sprintf(str, "Choose a move to replace with %s", data_moves[index - 39].name);
			text_Display(str);
			switch (text_AskQuestion4(data_moves[currentSave.party[i].moves[0]].name, data_moves[currentSave.party[i].moves[1]].name, data_moves[currentSave.party[i].moves[2]].name, data_moves[currentSave.party[i].moves[3]].name))
			{
			case 1:
				currentSave.party[i].moves[0] = index - 39;
				currentSave.party[i].pp[0] = data_moves[currentSave.party[i].moves[0]].uses;
				break;
			case 2:
				currentSave.party[i].moves[1] = index - 39;
				currentSave.party[i].pp[1] = data_moves[currentSave.party[i].moves[1]].uses;
				break;
			case 3:
				currentSave.party[i].moves[2] = index - 39;
				currentSave.party[i].pp[2] = data_moves[currentSave.party[i].moves[2]].uses;
				break;
			case 4:
				currentSave.party[i].moves[3] = index - 39;
				currentSave.party[i].pp[3] = data_moves[currentSave.party[i].moves[3]].uses;
				break;
			}
		}
		else {
			sprintf(str, "%s cannot learn %s", data_pokemon[currentSave.party[i].id].name, data_moves[index - 39].name);
			text_Display(str);
			return false;
		}
	}


	items_IndexToName(&itemName, index);
	sprintf(str, "Used a %s on %s", itemName, data_pokemon[currentSave.party[i].id].name);
	text_Display(str);
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
