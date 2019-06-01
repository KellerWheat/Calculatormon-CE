// convpng v6.8
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[38] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)306,
 (uint8_t*)464,
 (uint8_t*)714,
 (uint8_t*)793,
 (uint8_t*)865,
 (uint8_t*)941,
 (uint8_t*)1022,
 (uint8_t*)1106,
 (uint8_t*)1172,
 (uint8_t*)1255,
 (uint8_t*)1361,
 (uint8_t*)1454,
 (uint8_t*)1555,
 (uint8_t*)1649,
 (uint8_t*)1712,
 (uint8_t*)1786,
 (uint8_t*)1875,
 (uint8_t*)1964,
 (uint8_t*)2037,
 (uint8_t*)2130,
 (uint8_t*)2174,
 (uint8_t*)2210,
 (uint8_t*)2251,
 (uint8_t*)2292,
 (uint8_t*)2332,
 (uint8_t*)2384,
 (uint8_t*)2436,
 (uint8_t*)2494,
 (uint8_t*)2563,
 (uint8_t*)2657,
 (uint8_t*)3475,
 (uint8_t*)4750,
 (uint8_t*)5224,
 (uint8_t*)6597,
 (uint8_t*)6657,
};

bool PKMNSD5_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD5", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD5[0];
    for (i = 0; i < PKMNSD5_num; i++) {
        PKMNSD5[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
