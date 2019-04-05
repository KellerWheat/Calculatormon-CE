// convpng v6.8
#include <stdint.h>
#include "PKMNSD2.h"

#include <fileioc.h>
uint8_t *PKMNSD2[28] = {
 (uint8_t*)0,
 (uint8_t*)3138,
 (uint8_t*)4740,
 (uint8_t*)7878,
 (uint8_t*)9480,
 (uint8_t*)11786,
 (uint8_t*)14924,
 (uint8_t*)16526,
 (uint8_t*)18832,
 (uint8_t*)21970,
 (uint8_t*)23572,
 (uint8_t*)26710,
 (uint8_t*)29848,
 (uint8_t*)31450,
 (uint8_t*)33756,
 (uint8_t*)36894,
 (uint8_t*)38496,
 (uint8_t*)40802,
 (uint8_t*)42404,
 (uint8_t*)44710,
 (uint8_t*)47016,
 (uint8_t*)49322,
 (uint8_t*)52460,
 (uint8_t*)54062,
 (uint8_t*)57200,
 (uint8_t*)58802,
 (uint8_t*)61108,
 (uint8_t*)63414,
};

bool PKMNSD2_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD2", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD2[0];
    for (i = 0; i < PKMNSD2_num; i++) {
        PKMNSD2[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
