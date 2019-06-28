#ifndef MENU_H
#define MENU_H

void menu_Setup(void);

uint8_t menu_Menu(void);
int menu_PokemonMenu(bool pressEnter);
void menu_PokemonDetails(int pokemonIndex);
/* Returns -1 if no item is used */
int menu_Items(bool inBattle);
void menu_Box(void);
void menu_ItemShop(gfx_sprite_t *shopSprite);

extern gfx_sprite_t *typeIcons[17];

#endif