#ifndef ITEMS_H
#define ITEMS_H

void items_IndexToName(char *name, uint8_t index);
bool items_UseItem(uint8_t index);


extern char itemNames[20][16];
extern char itemDescriptions[20][68];
extern uint16_t itemPrices[20];
extern uint8_t itemCount;

#endif