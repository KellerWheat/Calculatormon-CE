#ifndef MAP_H
#define MAP_H

#include "data.h"

void map_Initialize(void);
void map_Setup(void);
int map_Loop(void);

/* Mallocs and decompresses sprites */
void map_SetupGfx(void);
/* Frees sprites */
void map_End(void);

/* Generate the pokeball tilemap */
void map_LoadPokeballs(void);
/* Puts player in front of starting pokemon center after losing a battle */
void map_LoseFight(void);
void map_WinFight(bool wild, uint16_t rewardMoney);

/* Used by menu */
void map_Draw(void);
/* Shows money and pokemon status*/
void map_DrawInformationBar(void);

extern uint8_t currentTrainer; 
extern uint8_t moveDir;

#endif