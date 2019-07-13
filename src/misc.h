#ifndef MISC_H
#define MISC_H

#include <stdint.h>

void Wait(uint16_t amount);
void SetColors(uint8_t gameState);
void FindColors(void);

uint8_t * color_1555ToRGB(uint16_t input);

void MallocIcons(void);
void FreeIcons(void);

extern char str[100];
extern bool debugging;
extern uint16_t i;
extern uint16_t e;

extern uint8_t colors[];

extern gfx_sprite_t *typeIcons[17];
extern gfx_sprite_t *statusIcons[5];
extern gfx_sprite_t *categoryIcons[3];




#endif