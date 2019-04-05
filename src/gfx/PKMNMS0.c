// convpng v6.8
#include <stdint.h>
#include "PKMNMS0.h"

#include <fileioc.h>
uint8_t *PKMNMS0[2] = {
 (uint8_t*)0,
 (uint8_t*)33024,
};

bool PKMNMS0_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNMS0", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNMS0[0];
    for (i = 0; i < PKMNMS0_num; i++) {
        PKMNMS0[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)PKMNMS0[0] - (unsigned int)tileset_tiles_data[0];
    for (i = 0; i < tileset_tiles_num; i++) {
        tileset_tiles_data[i] += data;
    }

    return (bool)appvar;
}
