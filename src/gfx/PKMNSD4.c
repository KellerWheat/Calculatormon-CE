// convpng v7.0
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[31] = {
 (uint8_t*)0,
 (uint8_t*)138,
 (uint8_t*)269,
 (uint8_t*)397,
 (uint8_t*)537,
 (uint8_t*)672,
 (uint8_t*)802,
 (uint8_t*)955,
 (uint8_t*)1104,
 (uint8_t*)1251,
 (uint8_t*)1399,
 (uint8_t*)1542,
 (uint8_t*)1688,
 (uint8_t*)1883,
 (uint8_t*)2078,
 (uint8_t*)2274,
 (uint8_t*)2473,
 (uint8_t*)2664,
 (uint8_t*)2858,
 (uint8_t*)3048,
 (uint8_t*)3242,
 (uint8_t*)3382,
 (uint8_t*)3524,
 (uint8_t*)3680,
 (uint8_t*)3829,
 (uint8_t*)4009,
 (uint8_t*)4188,
 (uint8_t*)4254,
 (uint8_t*)4393,
 (uint8_t*)12159,
 (uint8_t*)15524,
};

bool PKMNSD4_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD4", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD4[0];
    for (i = 0; i < PKMNSD4_num; i++) {
        PKMNSD4[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)PKMNSD4[27] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[28] - (unsigned int)outdoortileset_tiles_compressed[0];
    for (i = 0; i < outdoortileset_tiles_num; i++) {
        outdoortileset_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[29] - (unsigned int)indoortileset_tiles_compressed[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
