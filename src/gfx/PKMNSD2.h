// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD2__
#define __PKMNSD2__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD2_num 28

extern uint8_t *PKMNSD2[28];
#define c57 ((gfx_sprite_t*)PKMNSD2[0])
#define c58 ((gfx_sprite_t*)PKMNSD2[1])
#define c59 ((gfx_sprite_t*)PKMNSD2[2])
#define c60 ((gfx_sprite_t*)PKMNSD2[3])
#define c61 ((gfx_sprite_t*)PKMNSD2[4])
#define c62 ((gfx_sprite_t*)PKMNSD2[5])
#define c63 ((gfx_sprite_t*)PKMNSD2[6])
#define c64 ((gfx_sprite_t*)PKMNSD2[7])
#define c65 ((gfx_sprite_t*)PKMNSD2[8])
#define c66 ((gfx_sprite_t*)PKMNSD2[9])
#define c67 ((gfx_sprite_t*)PKMNSD2[10])
#define c68 ((gfx_sprite_t*)PKMNSD2[11])
#define c69 ((gfx_sprite_t*)PKMNSD2[12])
#define c70 ((gfx_sprite_t*)PKMNSD2[13])
#define c71 ((gfx_sprite_t*)PKMNSD2[14])
#define c72 ((gfx_sprite_t*)PKMNSD2[15])
#define c73 ((gfx_sprite_t*)PKMNSD2[16])
#define c74 ((gfx_sprite_t*)PKMNSD2[17])
#define c75 ((gfx_sprite_t*)PKMNSD2[18])
#define c76 ((gfx_sprite_t*)PKMNSD2[19])
#define c77 ((gfx_sprite_t*)PKMNSD2[20])
#define c78 ((gfx_sprite_t*)PKMNSD2[21])
#define c79 ((gfx_sprite_t*)PKMNSD2[22])
#define c80 ((gfx_sprite_t*)PKMNSD2[23])
#define c81 ((gfx_sprite_t*)PKMNSD2[24])
#define c82 ((gfx_sprite_t*)PKMNSD2[25])
#define c83 ((gfx_sprite_t*)PKMNSD2[26])
#define sizeof_battle_gfx_pal 502
#define battle_gfx_pal ((uint16_t*)PKMNSD2[27])
bool PKMNSD2_init(void);

#endif
