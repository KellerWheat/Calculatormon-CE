// convpng v7.0
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __map_gfx__
#define __map_gfx__
#include <stdint.h>

#define map_gfx_transparent_color_index 0

#define mapcursorsmall_width 7
#define mapcursorsmall_height 10
#define mapcursorsmall_size 72
extern uint8_t mapcursorsmall_data[72];
#define mapcursorsmall ((gfx_sprite_t*)mapcursorsmall_data)
#define shop_width 134
#define shop_height 84
#define shop_size 11258
extern uint8_t shop_compressed[98];

#endif
