#ifndef MISC_H
#define MISC_H

#include <stdint.h>

/* Multipurpose string variable */
extern char str[100];

extern uint16_t i1;
extern uint16_t i2;

extern uint16_t i;
extern uint16_t e;

extern uint8_t colors[];

void Wait(uint16_t amount);
void SetColors(uint8_t gameState);

#endif