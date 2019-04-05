// convpng v6.8
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __menu_gfx__
#define __menu_gfx__
#include <stdint.h>

#define menu_gfx_transparent_color_index 0

#define randomcolorsmenu_width 7
#define randomcolorsmenu_height 1
#define randomcolorsmenu_size 9
extern uint8_t randomcolorsmenu_data[9];
#define randomcolorsmenu ((gfx_sprite_t*)randomcolorsmenu_data)
#define menucursor_width 16
#define menucursor_height 16
#define menucursor_size 258
extern uint8_t menucursor_data[258];
#define menucursor ((gfx_sprite_t*)menucursor_data)
#define movecursor_width 16
#define movecursor_height 16
#define movecursor_size 258
extern uint8_t movecursor_data[258];
#define movecursor ((gfx_sprite_t*)movecursor_data)
#define sizeof_menu_gfx_pal 16
extern uint16_t menu_gfx_pal[8];

#endif
