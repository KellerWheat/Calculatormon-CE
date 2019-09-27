// convpng v7.0
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[10] = {
 (uint8_t*)0,
 (uint8_t*)7197,
 (uint8_t*)9782,
 (uint8_t*)12584,
 (uint8_t*)20690,
 (uint8_t*)28127,
 (uint8_t*)35015,
 (uint8_t*)42285,
 (uint8_t*)42351,
 (uint8_t*)42490,
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

    data = (unsigned int)PKMNSD4[0] - (unsigned int)indoortileset0_tiles_compressed[0];
    for (i = 0; i < indoortileset0_tiles_num; i++) {
        indoortileset0_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[1] - (unsigned int)indoortileset1_tiles_compressed[0];
    for (i = 0; i < indoortileset1_tiles_num; i++) {
        indoortileset1_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[2] - (unsigned int)indoortileset2_tiles_compressed[0];
    for (i = 0; i < indoortileset2_tiles_num; i++) {
        indoortileset2_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[3] - (unsigned int)outdoortileset0_tiles_compressed[0];
    for (i = 0; i < outdoortileset0_tiles_num; i++) {
        outdoortileset0_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[4] - (unsigned int)outdoortileset1_tiles_compressed[0];
    for (i = 0; i < outdoortileset1_tiles_num; i++) {
        outdoortileset1_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[5] - (unsigned int)outdoortileset2_tiles_compressed[0];
    for (i = 0; i < outdoortileset2_tiles_num; i++) {
        outdoortileset2_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[6] - (unsigned int)outdoortileset3_tiles_compressed[0];
    for (i = 0; i < outdoortileset3_tiles_num; i++) {
        outdoortileset3_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[8] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
