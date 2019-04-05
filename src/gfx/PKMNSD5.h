// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD5__
#define __PKMNSD5__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD5_num 21

extern uint8_t *PKMNSD5[21];
#define c132 ((gfx_sprite_t*)PKMNSD5[0])
#define c133 ((gfx_sprite_t*)PKMNSD5[1])
#define c134 ((gfx_sprite_t*)PKMNSD5[2])
#define c135 ((gfx_sprite_t*)PKMNSD5[3])
#define c136 ((gfx_sprite_t*)PKMNSD5[4])
#define c137 ((gfx_sprite_t*)PKMNSD5[5])
#define c138 ((gfx_sprite_t*)PKMNSD5[6])
#define c139 ((gfx_sprite_t*)PKMNSD5[7])
#define c140 ((gfx_sprite_t*)PKMNSD5[8])
#define c141 ((gfx_sprite_t*)PKMNSD5[9])
#define c142 ((gfx_sprite_t*)PKMNSD5[10])
#define c143 ((gfx_sprite_t*)PKMNSD5[11])
#define c144 ((gfx_sprite_t*)PKMNSD5[12])
#define c145 ((gfx_sprite_t*)PKMNSD5[13])
#define c146 ((gfx_sprite_t*)PKMNSD5[14])
#define c147 ((gfx_sprite_t*)PKMNSD5[15])
#define c148 ((gfx_sprite_t*)PKMNSD5[16])
#define c149 ((gfx_sprite_t*)PKMNSD5[17])
#define c150 ((gfx_sprite_t*)PKMNSD5[18])
#define c151 ((gfx_sprite_t*)PKMNSD5[19])
#define sizeof_battle_gfx_pal 502
#define battle_gfx_pal ((uint16_t*)PKMNSD5[20])
bool PKMNSD5_init(void);

#endif
