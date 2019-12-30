#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>

void battle_Initialize(void);
void battle_Setup(void);
int battle_Loop(void);
void battle_End(void);

void battle_SpawnWild(uint8_t id, uint8_t minlevel, uint8_t maxlevel);
void battle_SpawnTrainer(uint8_t ids[6][16], uint8_t levels[6][16], uint8_t trainernum);

extern struct pokemonData enemyParty[6];
extern bool textExit;

#endif