#ifndef ITEMS_H
#define ITEMS_H

void items_IndexToName(char *name, uint8_t index);
bool items_UseItem(uint8_t index);

extern char itemNames[][16];
//extern char itemDescriptions[][68];
extern uint16_t itemPrices[15];
extern uint8_t itemCount;

#endif