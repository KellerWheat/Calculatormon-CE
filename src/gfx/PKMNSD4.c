// convpng v7.0
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[7] = {
 (uint8_t*)0,
 (uint8_t*)66,
 (uint8_t*)205,
 (uint8_t*)8833,
 (uint8_t*)15981,
 (uint8_t*)23330,
 (uint8_t*)29871,
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

    data = (unsigned int)PKMNSD4[1] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[2] - (unsigned int)outdoortileset0_tiles_compressed[0];
    for (i = 0; i < outdoortileset0_tiles_num; i++) {
        outdoortileset0_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[3] - (unsigned int)outdoortileset1_tiles_compressed[0];
    for (i = 0; i < outdoortileset1_tiles_num; i++) {
        outdoortileset1_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[4] - (unsigned int)outdoortileset2_tiles_compressed[0];
    for (i = 0; i < outdoortileset2_tiles_num; i++) {
        outdoortileset2_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[5] - (unsigned int)indoortileset_tiles_compressed[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
