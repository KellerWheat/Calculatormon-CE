// convpng v6.8
#include <stdint.h>
#include "PKMNSD6.h"

#include <fileioc.h>
uint8_t *PKMNSD6[29] = {
 (uint8_t*)0,
 (uint8_t*)3,
 (uint8_t*)2309,
 (uint8_t*)4615,
 (uint8_t*)6921,
 (uint8_t*)9227,
 (uint8_t*)11533,
 (uint8_t*)13839,
 (uint8_t*)16145,
 (uint8_t*)18451,
 (uint8_t*)20757,
 (uint8_t*)23063,
 (uint8_t*)25369,
 (uint8_t*)27675,
 (uint8_t*)29981,
 (uint8_t*)32287,
 (uint8_t*)34593,
 (uint8_t*)36899,
 (uint8_t*)39205,
 (uint8_t*)41511,
 (uint8_t*)43817,
 (uint8_t*)46123,
 (uint8_t*)48429,
 (uint8_t*)50735,
 (uint8_t*)53041,
 (uint8_t*)55347,
 (uint8_t*)57653,
 (uint8_t*)59959,
 (uint8_t*)62265,
};

bool PKMNSD6_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD6", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD6[0];
    for (i = 0; i < PKMNSD6_num; i++) {
        PKMNSD6[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
