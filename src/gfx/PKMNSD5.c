// convpng v7.0
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[39] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)297,
 (uint8_t*)447,
 (uint8_t*)688,
 (uint8_t*)767,
 (uint8_t*)839,
 (uint8_t*)915,
 (uint8_t*)996,
 (uint8_t*)1080,
 (uint8_t*)1146,
 (uint8_t*)1229,
 (uint8_t*)1335,
 (uint8_t*)1428,
 (uint8_t*)1529,
 (uint8_t*)1623,
 (uint8_t*)1686,
 (uint8_t*)1760,
 (uint8_t*)1849,
 (uint8_t*)1938,
 (uint8_t*)2011,
 (uint8_t*)2104,
 (uint8_t*)2148,
 (uint8_t*)2184,
 (uint8_t*)2225,
 (uint8_t*)2266,
 (uint8_t*)2306,
 (uint8_t*)2358,
 (uint8_t*)2410,
 (uint8_t*)2468,
 (uint8_t*)2537,
 (uint8_t*)2631,
 (uint8_t*)3449,
 (uint8_t*)4581,
 (uint8_t*)5055,
 (uint8_t*)5854,
 (uint8_t*)6897,
 (uint8_t*)6957,
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
