// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD1__
#define __PKMNSD1__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD1_num 28

extern uint8_t *PKMNSD1[28];
#define c30 ((gfx_sprite_t*)PKMNSD1[0])
#define c31 ((gfx_sprite_t*)PKMNSD1[1])
#define c32 ((gfx_sprite_t*)PKMNSD1[2])
#define c33 ((gfx_sprite_t*)PKMNSD1[3])
#define c34 ((gfx_sprite_t*)PKMNSD1[4])
#define c35 ((gfx_sprite_t*)PKMNSD1[5])
#define c36 ((gfx_sprite_t*)PKMNSD1[6])
#define c37 ((gfx_sprite_t*)PKMNSD1[7])
#define c38 ((gfx_sprite_t*)PKMNSD1[8])
#define c39 ((gfx_sprite_t*)PKMNSD1[9])
#define c40 ((gfx_sprite_t*)PKMNSD1[10])
#define c41 ((gfx_sprite_t*)PKMNSD1[11])
#define c42 ((gfx_sprite_t*)PKMNSD1[12])
#define c43 ((gfx_sprite_t*)PKMNSD1[13])
#define c44 ((gfx_sprite_t*)PKMNSD1[14])
#define c45 ((gfx_sprite_t*)PKMNSD1[15])
#define c46 ((gfx_sprite_t*)PKMNSD1[16])
#define c47 ((gfx_sprite_t*)PKMNSD1[17])
#define c48 ((gfx_sprite_t*)PKMNSD1[18])
#define c49 ((gfx_sprite_t*)PKMNSD1[19])
#define c50 ((gfx_sprite_t*)PKMNSD1[20])
#define c51 ((gfx_sprite_t*)PKMNSD1[21])
#define c52 ((gfx_sprite_t*)PKMNSD1[22])
#define c53 ((gfx_sprite_t*)PKMNSD1[23])
#define c54 ((gfx_sprite_t*)PKMNSD1[24])
#define c55 ((gfx_sprite_t*)PKMNSD1[25])
#define c56 ((gfx_sprite_t*)PKMNSD1[26])
#define sizeof_battle_gfx_pal 502
#define battle_gfx_pal ((uint16_t*)PKMNSD1[27])
bool PKMNSD1_init(void);

#endif
