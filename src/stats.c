#include <math.h>

#include "stats.h"

unsigned long isqrt(unsigned long x);

struct pokemonStats stats_CalculateStats(struct pokemonData c) {
	struct pokemonStats newstats;
	newstats.health = ((((data_pokemon[c.id].basehealth + c.healthIV) * 2 + (isqrt(c.healthEV) / 4))*c.level) / 100) + c.level + 10;
	newstats.attack = ((((data_pokemon[c.id].baseattack + c.attackIV) * 2 + (isqrt(c.attackEV) / 4))*c.level) / 100) + 5;
	newstats.defence = ((((data_pokemon[c.id].basedefence + c.defenceIV) * 2 + (isqrt(c.defenceEV) / 4))*c.level) / 100) + 5;
	newstats.specialattack = ((((data_pokemon[c.id].basespecialattack + c.specialattackIV) * 2 + (isqrt(c.specialattackEV) / 4))*c.level) / 100) + 5;
	newstats.specialdefence = ((((data_pokemon[c.id].basespecialdefence + c.specialdefenceIV) * 2 + (isqrt(c.specialdefenceEV) / 4))*c.level) / 100) + 5;
	newstats.speed = ((((data_pokemon[c.id].basespeed + c.speedIV) * 2 + (isqrt(c.speedEV) / 4))*c.level) / 100) + 5;
	return newstats;
}
struct pokemonData stats_NewCharacter(uint8_t id, uint8_t level) {
	uint8_t parent1id = data_pokemon[id].evolvesfrom;
	uint8_t parent2id = data_pokemon[parent1id].evolvesfrom;
	int moveSlot, moveIndex, statusIndex;
	struct pokemonData newch;
	newch.id = id;
	newch.level = level;
	newch.xp = xpPerLevel[data_pokemon[newch.id].xptype][newch.level];

	newch.move1 = 0;
	newch.move2 = 0;
	newch.move3 = 0;
	newch.move4 = 0;
	moveSlot = 0;
	for (moveIndex = 0; moveIndex < 10; moveIndex++) {
		if (data_pokemon[parent2id].movelevels[moveIndex] <= newch.level && data_pokemon[parent2id].moveids[moveIndex] != 0) {
			if (moveSlot == 0) {
				newch.move1 = data_pokemon[parent2id].moveids[moveIndex];
			}
			if (moveSlot == 1) {
				newch.move2 = data_pokemon[parent2id].moveids[moveIndex];
			}
			if (moveSlot == 2) {
				newch.move3 = data_pokemon[parent2id].moveids[moveIndex];
			}
			if (moveSlot == 3) {
				newch.move4 = data_pokemon[parent2id].moveids[moveIndex];
			}
			moveSlot++;
			if (moveSlot == 4) {
				moveSlot = 0;
			}
		}
	}
	for (moveIndex = 0; moveIndex < 10; moveIndex++) {
		if (data_pokemon[parent1id].movelevels[moveIndex] <= newch.level && data_pokemon[parent1id].moveids[moveIndex] != 0) {
			if (moveSlot == 0) {
				newch.move1 = data_pokemon[parent1id].moveids[moveIndex];
			}
			if (moveSlot == 1) {
				newch.move2 = data_pokemon[parent1id].moveids[moveIndex];
			}
			if (moveSlot == 2) {
				newch.move3 = data_pokemon[parent1id].moveids[moveIndex];
			}
			if (moveSlot == 3) {
				newch.move4 = data_pokemon[parent1id].moveids[moveIndex];
			}
			moveSlot++;
			if (moveSlot == 4) {
				moveSlot = 0;
			}
		}
	}
	for (moveIndex = 0; moveIndex < 10; moveIndex++) {
		if (data_pokemon[id].movelevels[moveIndex] <= newch.level && data_pokemon[id].moveids[moveIndex] != 0) {
			if (moveSlot == 0) {
				newch.move1 = data_pokemon[id].moveids[moveIndex];
			}
			if (moveSlot == 1) {
				newch.move2 = data_pokemon[id].moveids[moveIndex];
			}
			if (moveSlot == 2) {
				newch.move3 = data_pokemon[id].moveids[moveIndex];
			}
			if (moveSlot == 3) {
				newch.move4 = data_pokemon[id].moveids[moveIndex];
			}
			moveSlot++;
			if (moveSlot == 4) {
				moveSlot = 0;
			}
		}
	}

	for (statusIndex = 0; statusIndex < 5; statusIndex++) {
		newch.currentstatus[statusIndex] = 0;
	}

	newch.healthIV = (rand() % 16);
	newch.attackIV = (rand() % 16);
	newch.defenceIV = (rand() % 16);
	newch.specialattackIV = (rand() % 16);
	newch.specialdefenceIV = (rand() % 16);
	newch.speedIV = (rand() % 16);
	newch.healthEV = 0;
	newch.attackEV = 0;
	newch.defenceEV = 0;
	newch.specialattackEV = 0;
	newch.specialdefenceEV = 0;
	newch.speedEV = 0;

	newch.currenthealth = ((((data_pokemon[newch.id].basehealth + newch.healthIV) * 2 + (isqrt(newch.healthEV) / 4))*newch.level) / 100) + newch.level + 10;

	return newch;
}

/* NOTE: I have no idea how this works */
unsigned long isqrt(unsigned long x)
{
	
	register unsigned long op, res, one;

	op = x;
	res = 0;

	/* "one" starts at the highest power of four <= than the argument. */
	one = 1 << 30;  /* second-to-top bit set */
	while (one > op) one >>= 2;

	while (one != 0) {
		if (op >= res + one) {
			op -= res + one;
			res += one << 1;  // <-- faster than 2 * one  
		}
		res >>= 1;
		one >>= 2;
	}
	return res;
}