#ifndef MAP_H
#define MAP_H

void map_Initialize(void);
void map_Setup(void);
int map_Loop(void);

/* Puts player in front of starting pokemon center after losing a battle */
void map_Respawn(void);
/* Shows money and pokemon status*/
void map_DrawInformationBar(void);

extern uint8_t currentTrainer; 
extern uint8_t moveDir;

#endif