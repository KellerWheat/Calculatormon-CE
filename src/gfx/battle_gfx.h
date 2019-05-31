// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __battle_gfx__
#define __battle_gfx__
#include <stdint.h>

#define battle_gfx_transparent_color_index 0

#define menucursor_width 13
#define menucursor_height 20
#define menucursor_size 262
extern uint8_t menucursor_data[262];
#define menucursor ((gfx_sprite_t*)menucursor_data)
#define menucursorsmall_width 7
#define menucursorsmall_height 10
#define menucursorsmall_size 72
extern uint8_t menucursorsmall_data[72];
#define menucursorsmall ((gfx_sprite_t*)menucursorsmall_data)

#endif
