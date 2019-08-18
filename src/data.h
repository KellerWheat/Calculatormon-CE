 /* This contains all the data that can be edited */

#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#include <graphx.h>
#include "gfx/battle_gfx.h"

/* Structs */

struct pokemonData {
	uint8_t id;
	uint8_t moves[4];
	uint8_t pp[4];
	uint8_t level;
	int xp;

	uint16_t currenthealth;
	uint8_t currentstatus;

	uint8_t healthIV;
	uint8_t attackIV;
	uint8_t defenceIV;
	uint8_t specialattackIV;
	uint8_t specialdefenceIV;
	uint8_t speedIV;
	uint16_t healthEV;
	uint16_t attackEV;
	uint16_t defenceEV;
	uint16_t specialattackEV;
	uint16_t specialdefenceEV;
	uint16_t speedEV;
};
struct pokemonType {
	char name[16];
	uint8_t element1;
	uint8_t element2;
	uint8_t basehealth;
	uint8_t baseattack;
	uint8_t basedefence;
	uint8_t basespecialattack;
	uint8_t basespecialdefence;
	uint8_t basespeed;
	uint8_t movelevels[10];
	uint8_t moveids[10];
	uint8_t evolvelevel;
	uint8_t evolveid;
	uint8_t evolvesfrom;
	uint16_t xpdrop;
	uint8_t xptype;
	uint8_t catchrate;
};
struct pokemonStats {
	uint16_t health;
	uint16_t attack;
	uint16_t defence;
	uint16_t specialattack;
	uint16_t specialdefence;
	uint16_t speed;
};
struct move {
	char name[14];
	uint8_t element;
	uint8_t type;
	uint8_t power;
	uint8_t accuracy;
	uint8_t uses;
	uint8_t statustype;
	uint8_t statuschance;
};
struct zoneData {
	uint8_t spawnids[5][16];
	uint8_t spawnminlevels[5][16];
	uint8_t spawnmaxlevels[5][16];

	uint8_t exitzone[16];
	uint8_t exitx[16];
	uint8_t exity[16];

	uint8_t doorzones[16];
	uint8_t doorx[16];
	uint8_t doory[16];

	uint8_t textIds[16];

	uint8_t trainerspawnids[6][16];
	uint8_t trainerspawnlevels[6][16];
	uint8_t trainerx[16];
	uint8_t trainery[16];
	uint8_t trainerdir[16];
	uint8_t trainerskin[16];
	uint8_t trainertype[16];
	uint8_t trainermovement[16];
	uint8_t trainertext[16];
	uint8_t trainerreward[16];//Reward is an item (number is one more than actual id)
};

/* Data from other structs */
void LoadText(int index);
extern char loadedText[256];

extern struct pokemonType data_pokemon[];
extern struct move data_moves[];
extern char data_moveDescriptions[][78];
extern uint8_t elements[18][18];
extern int xpPerLevel[4][101];

extern char data_text[][32];
extern uint8_t data_tileSets[32];

#endif

