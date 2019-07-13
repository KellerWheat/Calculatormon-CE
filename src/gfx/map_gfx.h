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
#define grassoverlay1_width 16
#define grassoverlay1_height 6
#define grassoverlay1_size 98
extern uint8_t grassoverlay1_data[98];
#define grassoverlay1 ((gfx_sprite_t*)grassoverlay1_data)
#define grassoverlay2_width 16
#define grassoverlay2_height 10
#define grassoverlay2_size 162
extern uint8_t grassoverlay2_data[162];
#define grassoverlay2 ((gfx_sprite_t*)grassoverlay2_data)
#define grassoverlay3_width 16
#define grassoverlay3_height 11
#define grassoverlay3_size 178
extern uint8_t grassoverlay3_data[178];
#define grassoverlay3 ((gfx_sprite_t*)grassoverlay3_data)
#define animatedwater1_width 16
#define animatedwater1_height 16
#define animatedwater1_size 258
extern uint8_t animatedwater1_data[258];
#define animatedwater1 ((gfx_sprite_t*)animatedwater1_data)
#define animatedwater2_width 16
#define animatedwater2_height 16
#define animatedwater2_size 258
extern uint8_t animatedwater2_data[258];
#define animatedwater2 ((gfx_sprite_t*)animatedwater2_data)
#define shop_width 134
#define shop_height 84
#define shop_size 11258
extern uint8_t shop_compressed[98];

#endif
