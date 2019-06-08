// convpng v6.8
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[23] = {
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
 (uint8_t*)1828,
 (uint8_t*)1970,
 (uint8_t*)2126,
 (uint8_t*)2275,
 (uint8_t*)2455,
 (uint8_t*)2634,
 (uint8_t*)2700,
 (uint8_t*)2839,
 (uint8_t*)9827,
 (uint8_t*)13192,
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

    data = (unsigned int)PKMNSD4[19] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[20] - (unsigned int)outdoortileset_tiles_compressed[0];
    for (i = 0; i < outdoortileset_tiles_num; i++) {
        outdoortileset_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[21] - (unsigned int)indoortileset_tiles_compressed[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
