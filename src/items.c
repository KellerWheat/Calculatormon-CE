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


void items_ItemShop(void) {
	uint8_t shopPage = 0;
	uint8_t shopCurrent = 1;

	gfx_SetDrawScreen();
reloadshop:

	gfx_SetColor(colors[0]);
	gfx_FillRectangle(20, 20, 290, 200);
	gfx_SetColor(colors[1]);
	gfx_Rectangle(20, 20, 290, 200);
	gfx_SetColor(colors[0]);
	gfx_PrintStringXY(">", 25, 25 + shopCurrent * 20);
	map_DrawInformationBar();
	while (kb_Data[1] & kb_2nd) { kb_Scan(); }

	i = 0;
	while (i < 10) {
		gfx_PrintStringXY(itemNames[i + shopPage], 35, 25 + i * 20);
		sprintf(str, "%u$", itemPrices[i + shopPage]);
		gfx_PrintStringXY(str, 170, 25 + i * 20);
		sprintf(str, "%u", playerItems[i + shopPage]);
		gfx_PrintStringXY(str, 240, 25 + i * 20);
		i++;
	}

	while (!(kb_Data[6] & kb_Clear))
	{
		kb_Scan();
		if (kb_Data[7] & kb_Up) {
			if (shopCurrent != 0) {
				shopCurrent--;
				gfx_FillRectangle(25, 25, 9, 190);
				gfx_PrintStringXY(">", 25, 25 + shopCurrent * 20);
				Wait(20);
			}
		}
		if (kb_Data[7] & kb_Down) {
			if (shopCurrent != 9) {
				shopCurrent++;
				gfx_FillRectangle(25, 25, 9, 190);
				gfx_PrintStringXY(">", 25, 25 + shopCurrent * 20);
				Wait(20);
			}
		}
		if ((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
			if (shopPage == 10) {
				shopPage = 0;
			}
			else {
				shopPage = 10;
			}
			shopCurrent = 0;
			gfx_FillRectangle(25, 25, 9, 190);
			gfx_PrintStringXY(">", 25, 25 + shopCurrent * 20);
			Wait(20);
			goto reloadshop;
		}

		if (kb_Data[1] & kb_2nd) {
			if (playerMoney >= itemPrices[shopCurrent + shopPage]) {
				playerMoney -= itemPrices[shopCurrent + shopPage];
				playerItems[shopCurrent + shopPage]++;
			}
			while (kb_Data[1] & kb_2nd) { kb_Scan(); }
			goto reloadshop;
		}
	}
	gfx_SetDrawBuffer();
}

bool items_UseItem(uint8_t index) {
	char itemName[20];
	if (index < 4 || playerItems[index] == 0) {
		return false;
	}
	i = menu_PokemonMenu(false);
	if (i == 0 || party[i - 1].id == 0 || party[i - 1].currenthealth == 0) {
		return false;
	}
	i -= 1;
	if (index < 8) {
		playerItems[index]--;
		HealHp(i, healAmounts[index - 4]);
	}
	else if (index < 10) {
		playerItems[index]--;
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
		playerItems[index]--;
		HealStatus(i, index-10);
	}
	else if (index < 20) {
		sprintf(str, "%s has evolved", data_pokemon[party[i].id].name);
		if (data_pokemon[party[i].id].evolvelevel == index + 86) {
			party[i].id = data_pokemon[party[i].id].evolveid;
		}
		else if (data_pokemon[party[i].id].evolvelevel == 106) {
			if (index = 19) {
				party[i].id += 1;
			}
			else if (index = 17) {
				party[i].id += 2;
			}
			else if (index = 15) {
				party[i].id += 3;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
		playerItems[index]--;
		text_Display(str, false);
		return true;
	}

	if (index >= 20) {
		if (data_pokemon[party[i].id].element1 == data_moves[index - 19].element || data_pokemon[party[i].id].element2 == data_moves[index - 19].element) {
			sprintf(str, "Choose a move to replace with %s", data_moves[index - 19].name);
			text_Display(str, false);
			switch (text_AskQuestion4(data_moves[party[i].moves[0]].name, data_moves[party[i].moves[1]].name, data_moves[party[i].moves[2]].name, data_moves[party[i].moves[3]].name, false))
			{
			case 1:
				party[i].moves[0] = index - 19;
				break;
			case 2:
				party[i].moves[1] = index - 19;
				break;
			case 3:
				party[i].moves[2] = index - 19;
				break;
			case 4:
				party[i].moves[3] = index - 19;
				break;
			}
		}
		else {
			sprintf(str, "%s cannot learn %s", data_pokemon[party[i].id].name, data_moves[index - 19].name);
			text_Display(str, false);
			return false;
		}
	}


	items_IndexToName(&itemName, index);
	sprintf(str, "Used a %s on %s", itemName, data_pokemon[party[i].id].name);
	text_Display(str, true);
	return true;
}

void HealHp(uint8_t index, uint16_t amount) {
	party[index].currenthealth += amount;
	if (party[index].currenthealth > stats_CalculateStats(party[index]).health) {
		party[index].currenthealth = stats_CalculateStats(party[index]).health;
	}
}
void HealStatus(uint8_t index, uint8_t type) {
	party[index].currentstatus = 0;
}
