// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD3__
#define __PKMNSD3__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD3_num 26

extern uint8_t *PKMNSD3[26];
#define c84 ((gfx_sprite_t*)PKMNSD3[0])
#define c85 ((gfx_sprite_t*)PKMNSD3[1])
#define c86 ((gfx_sprite_t*)PKMNSD3[2])
#define c87 ((gfx_sprite_t*)PKMNSD3[3])
#define c88 ((gfx_sprite_t*)PKMNSD3[4])
#define c89 ((gfx_sprite_t*)PKMNSD3[5])
#define c90 ((gfx_sprite_t*)PKMNSD3[6])
#define c91 ((gfx_sprite_t*)PKMNSD3[7])
#define c92 ((gfx_sprite_t*)PKMNSD3[8])
#define c93 ((gfx_sprite_t*)PKMNSD3[9])
#define c94 ((gfx_sprite_t*)PKMNSD3[10])
#define c95 ((gfx_sprite_t*)PKMNSD3[11])
#define c96 ((gfx_sprite_t*)PKMNSD3[12])
#define c97 ((gfx_sprite_t*)PKMNSD3[13])
#define c98 ((gfx_sprite_t*)PKMNSD3[14])
#define c99 ((gfx_sprite_t*)PKMNSD3[15])
#define c100 ((gfx_sprite_t*)PKMNSD3[16])
#define c101 ((gfx_sprite_t*)PKMNSD3[17])
#define c102 ((gfx_sprite_t*)PKMNSD3[18])
#define c103 ((gfx_sprite_t*)PKMNSD3[19])
#define c104 ((gfx_sprite_t*)PKMNSD3[20])
#define c105 ((gfx_sprite_t*)PKMNSD3[21])
#define c106 ((gfx_sprite_t*)PKMNSD3[22])
#define c107 ((gfx_sprite_t*)PKMNSD3[23])
#define c108 ((gfx_sprite_t*)PKMNSD3[24])
#define sizeof_battle_gfx_pal 502
#define battle_gfx_pal ((uint16_t*)PKMNSD3[25])
bool PKMNSD3_init(void);

#endif
