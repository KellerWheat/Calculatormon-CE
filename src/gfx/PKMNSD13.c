// convpng v6.8
#include <stdint.h>
#include "PKMNSD13.h"

#include <fileioc.h>
uint8_t *PKMNSD13[2] = {
 (uint8_t*)0,
 (uint8_t*)33024,
};

bool PKMNSD13_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD13", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD13[0];
    for (i = 0; i < PKMNSD13_num; i++) {
        PKMNSD13[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)PKMNSD13[0] - (unsigned int)indoortileset_tiles_data[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_data[i] += data;
    }

    return (bool)appvar;
}
