// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __PKMNSD5__
#define __PKMNSD5__
#include <stdint.h>

#include <stdbool.h>

#define PKMNSD5_num 38

extern uint8_t *PKMNSD5[38];
#define menutextbox1_compressed ((gfx_sprite_t*)PKMNSD5[0])
#define menutextbox2_compressed ((gfx_sprite_t*)PKMNSD5[1])
#define battletextbox1_compressed ((gfx_sprite_t*)PKMNSD5[2])
#define battletextbox2_compressed ((gfx_sprite_t*)PKMNSD5[3])
#define battletextbox3_compressed ((gfx_sprite_t*)PKMNSD5[4])
#define type00_compressed ((gfx_sprite_t*)PKMNSD5[5])
#define type01_compressed ((gfx_sprite_t*)PKMNSD5[6])
#define type02_compressed ((gfx_sprite_t*)PKMNSD5[7])
#define type03_compressed ((gfx_sprite_t*)PKMNSD5[8])
#define type04_compressed ((gfx_sprite_t*)PKMNSD5[9])
#define type05_compressed ((gfx_sprite_t*)PKMNSD5[10])
#define type06_compressed ((gfx_sprite_t*)PKMNSD5[11])
#define type07_compressed ((gfx_sprite_t*)PKMNSD5[12])
#define type08_compressed ((gfx_sprite_t*)PKMNSD5[13])
#define type09_compressed ((gfx_sprite_t*)PKMNSD5[14])
#define type10_compressed ((gfx_sprite_t*)PKMNSD5[15])
#define type11_compressed ((gfx_sprite_t*)PKMNSD5[16])
#define type12_compressed ((gfx_sprite_t*)PKMNSD5[17])
#define type13_compressed ((gfx_sprite_t*)PKMNSD5[18])
#define type14_compressed ((gfx_sprite_t*)PKMNSD5[19])
#define type15_compressed ((gfx_sprite_t*)PKMNSD5[20])
#define type16_compressed ((gfx_sprite_t*)PKMNSD5[21])
#define status1_compressed ((gfx_sprite_t*)PKMNSD5[22])
#define status2_compressed ((gfx_sprite_t*)PKMNSD5[23])
#define status3_compressed ((gfx_sprite_t*)PKMNSD5[24])
#define status4_compressed ((gfx_sprite_t*)PKMNSD5[25])
#define status5_compressed ((gfx_sprite_t*)PKMNSD5[26])
#define category0_compressed ((gfx_sprite_t*)PKMNSD5[27])
#define category1_compressed ((gfx_sprite_t*)PKMNSD5[28])
#define category2_compressed ((gfx_sprite_t*)PKMNSD5[29])
#define pokemonlist_compressed ((gfx_sprite_t*)PKMNSD5[30])
#define menupokemon_compressed ((gfx_sprite_t*)PKMNSD5[31])
#define bag_compressed ((gfx_sprite_t*)PKMNSD5[32])
#define infoscreen1_compressed ((gfx_sprite_t*)PKMNSD5[33])
#define infoscreen2_compressed ((gfx_sprite_t*)PKMNSD5[34])
#define background_compressed ((gfx_sprite_t*)PKMNSD5[35])
#define hpbar_compressed ((gfx_sprite_t*)PKMNSD5[36])
#define sizeof_battle_gfx_pal 512
#define battle_gfx_pal ((uint16_t*)PKMNSD5[37])
bool PKMNSD5_init(void);

#endif
