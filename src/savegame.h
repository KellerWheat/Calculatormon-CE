#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <stdbool.h>
#include <stdint.h>

extern const char appVarName[];

#define AREACOUNT 16
#define BUILDINGCOUNT 16

struct saveGame
{
	bool newGame;
	uint8_t rivalPokemon;
	uint16_t playerX;
	uint16_t playerY;
	uint16_t lastPlayerX;
	uint16_t lastPlayerY;
	struct pokemonData party[36];
	uint16_t currentZone;
	bool indoors;
	bool surfing;
	uint8_t currentBuilding;
	uint32_t playerMoney;
	uint8_t playerItems[185];
	bool defeatedTrainers[AREACOUNT][16];
	bool defeatedTrainersIndoors[BUILDINGCOUNT][16];
	bool takenGifts[AREACOUNT][16];
	bool takenGiftsIndoors[BUILDINGCOUNT][16];
	bool takenGroundGifts[2][AREACOUNT][16];
	uint16_t foundItemsPos[2][AREACOUNT][16];
	uint8_t badgeCount;
	uint8_t pokedex[151];
	uint16_t worldState;
};

extern struct saveGame currentSave;


void save_Save(void);
void save_Load(void);
void save_SelectSave(void);


#endif