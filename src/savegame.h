#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <stdbool.h>
#include <stdint.h>

extern const char appVarName[];

//struct saveGame
//{
//
//};

extern bool newGame;
extern uint8_t rivalPokemon;
extern uint16_t playerX;
extern uint16_t playerY;
extern uint16_t lastPlayerX;
extern uint16_t lastPlayerY;
extern struct pokemonData party[36];
extern uint16_t currentZone;
extern bool indoors;
extern bool surfing;
extern uint8_t currentBuilding;
extern uint32_t playerMoney;
extern uint8_t playerItems[185];
extern bool defeatedTrainers[][4];
extern bool defeatedTrainersIndoors[][4];
extern bool takenGifts[][16];
extern bool takenGiftsIndoors[][16];
extern uint8_t badgeCount;


void save_Save(void);
void save_Load(void);
void save_SelectSave(void);


#endif