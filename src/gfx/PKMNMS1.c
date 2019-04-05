// convpng v6.8
#include <stdint.h>
#include "PKMNMS1.h"

#include <fileioc.h>
uint8_t *PKMNMS1[2] = {
 (uint8_t*)0,
 (uint8_t*)33024,
};

bool PKMNMS1_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNMS1", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNMS1[0];
    for (i = 0; i < PKMNMS1_num; i++) {
        PKMNMS1[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)PKMNMS1[0] - (unsigned int)indoortileset_tiles_data[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_data[i] += data;
    }

    return (bool)appvar;
}
