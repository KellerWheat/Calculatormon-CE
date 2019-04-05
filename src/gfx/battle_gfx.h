// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __battle_gfx__
#define __battle_gfx__
#include <stdint.h>

#define battle_gfx_transparent_color_index 0

#define randomcolorsbattle_width 7
#define randomcolorsbattle_height 1
#define randomcolorsbattle_size 9
extern uint8_t randomcolorsbattle_data[9];
#define randomcolorsbattle ((gfx_sprite_t*)randomcolorsbattle_data)

#endif
