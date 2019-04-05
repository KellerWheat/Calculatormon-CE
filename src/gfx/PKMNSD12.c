// convpng v6.8
#include <stdint.h>
#include "PKMNSD12.h"

#include <fileioc.h>
uint8_t *PKMNSD12[2] = {
 (uint8_t*)0,
 (uint8_t*)33024,
};

bool PKMNSD12_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD12", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD12[0];
    for (i = 0; i < PKMNSD12_num; i++) {
        PKMNSD12[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)PKMNSD12[0] - (unsigned int)outdoortileset_tiles_data[0];
    for (i = 0; i < outdoortileset_tiles_num; i++) {
        outdoortileset_tiles_data[i] += data;
    }

    return (bool)appvar;
}
