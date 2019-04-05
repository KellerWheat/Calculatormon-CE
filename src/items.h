#ifndef ITEMS_H
#define ITEMS_H

void items_IndexToName(char *name, uint8_t index);
void items_ItemShop(void);
bool items_UseItem(uint8_t index);

extern char itemNames[20][16];
extern uint16_t itemPrices[20];
extern uint8_t itemCount;

#endif