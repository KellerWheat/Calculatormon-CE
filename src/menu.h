#ifndef MENU_H
#define MENU_H

void menu_Setup(void);

uint8_t menu_Menu(void);
int menu_PokemonMenu(bool pressEnter);
void menu_PokemonDetails(int pokemonIndex);
bool menu_Items(bool inBattle);

extern gfx_sprite_t *typeIcons[17];

#endif