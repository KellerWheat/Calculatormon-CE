// convpng v7.0
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[43] = {
 (uint8_t*)0,
 (uint8_t*)138,
 (uint8_t*)269,
 (uint8_t*)397,
 (uint8_t*)537,
 (uint8_t*)672,
 (uint8_t*)802,
 (uint8_t*)954,
 (uint8_t*)1103,
 (uint8_t*)1250,
 (uint8_t*)1397,
 (uint8_t*)1539,
 (uint8_t*)1685,
 (uint8_t*)1816,
 (uint8_t*)1943,
 (uint8_t*)2082,
 (uint8_t*)2216,
 (uint8_t*)2346,
 (uint8_t*)2491,
 (uint8_t*)2621,
 (uint8_t*)2751,
 (uint8_t*)2880,
 (uint8_t*)3022,
 (uint8_t*)3158,
 (uint8_t*)3292,
 (uint8_t*)3487,
 (uint8_t*)3682,
 (uint8_t*)3878,
 (uint8_t*)4077,
 (uint8_t*)4268,
 (uint8_t*)4462,
 (uint8_t*)4652,
 (uint8_t*)4846,
 (uint8_t*)4986,
 (uint8_t*)5128,
 (uint8_t*)5284,
 (uint8_t*)5433,
 (uint8_t*)5613,
 (uint8_t*)5792,
 (uint8_t*)5858,
 (uint8_t*)5997,
 (uint8_t*)13758,
 (uint8_t*)17123,
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

    data = (unsigned int)PKMNSD4[39] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[40] - (unsigned int)outdoortileset_tiles_compressed[0];
    for (i = 0; i < outdoortileset_tiles_num; i++) {
        outdoortileset_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[41] - (unsigned int)indoortileset_tiles_compressed[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
