#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <graphx.h>
#include "data.h"	

struct pokemonStats stats_CalculateStats(struct pokemonData c);
struct pokemonData stats_NewCharacter(uint8_t id, uint8_t level);

#endif