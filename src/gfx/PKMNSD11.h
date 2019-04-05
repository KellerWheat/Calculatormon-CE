// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD11__
#define __PKMNSD11__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD11_num 13

extern uint8_t *PKMNSD11[13];
#define cb140 ((gfx_sprite_t*)PKMNSD11[0])
#define cb141 ((gfx_sprite_t*)PKMNSD11[1])
#define cb142 ((gfx_sprite_t*)PKMNSD11[2])
#define cb143 ((gfx_sprite_t*)PKMNSD11[3])
#define cb144 ((gfx_sprite_t*)PKMNSD11[4])
#define cb145 ((gfx_sprite_t*)PKMNSD11[5])
#define cb146 ((gfx_sprite_t*)PKMNSD11[6])
#define cb147 ((gfx_sprite_t*)PKMNSD11[7])
#define cb148 ((gfx_sprite_t*)PKMNSD11[8])
#define cb149 ((gfx_sprite_t*)PKMNSD11[9])
#define cb150 ((gfx_sprite_t*)PKMNSD11[10])
#define cb151 ((gfx_sprite_t*)PKMNSD11[11])
#define sizeof_battle_gfx_pal 502
#define battle_gfx_pal ((uint16_t*)PKMNSD11[12])
bool PKMNSD11_init(void);

#endif
