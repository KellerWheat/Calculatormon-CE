// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD6__
#define __PKMNSD6__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD6_num 29

extern uint8_t *PKMNSD6[29];
#define cb0 ((gfx_sprite_t*)PKMNSD6[0])
#define cb1 ((gfx_sprite_t*)PKMNSD6[1])
#define cb2 ((gfx_sprite_t*)PKMNSD6[2])
#define cb3 ((gfx_sprite_t*)PKMNSD6[3])
#define cb4 ((gfx_sprite_t*)PKMNSD6[4])
#define cb5 ((gfx_sprite_t*)PKMNSD6[5])
#define cb6 ((gfx_sprite_t*)PKMNSD6[6])
#define cb7 ((gfx_sprite_t*)PKMNSD6[7])
#define cb8 ((gfx_sprite_t*)PKMNSD6[8])
#define cb9 ((gfx_sprite_t*)PKMNSD6[9])
#define cb10 ((gfx_sprite_t*)PKMNSD6[10])
#define cb11 ((gfx_sprite_t*)PKMNSD6[11])
#define cb12 ((gfx_sprite_t*)PKMNSD6[12])
#define cb13 ((gfx_sprite_t*)PKMNSD6[13])
#define cb14 ((gfx_sprite_t*)PKMNSD6[14])
#define cb15 ((gfx_sprite_t*)PKMNSD6[15])
#define cb16 ((gfx_sprite_t*)PKMNSD6[16])
#define cb17 ((gfx_sprite_t*)PKMNSD6[17])
#define cb18 ((gfx_sprite_t*)PKMNSD6[18])
#define cb19 ((gfx_sprite_t*)PKMNSD6[19])
#define cb20 ((gfx_sprite_t*)PKMNSD6[20])
#define cb21 ((gfx_sprite_t*)PKMNSD6[21])
#define cb22 ((gfx_sprite_t*)PKMNSD6[22])
#define cb23 ((gfx_sprite_t*)PKMNSD6[23])
#define cb24 ((gfx_sprite_t*)PKMNSD6[24])
#define cb25 ((gfx_sprite_t*)PKMNSD6[25])
#define cb26 ((gfx_sprite_t*)PKMNSD6[26])
#define cb27 ((gfx_sprite_t*)PKMNSD6[27])
#define sizeof_battle_gfx_pal 502
#define battle_gfx_pal ((uint16_t*)PKMNSD6[28])
bool PKMNSD6_init(void);

#endif
