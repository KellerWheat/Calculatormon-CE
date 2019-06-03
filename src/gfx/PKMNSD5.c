// convpng v6.8
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[38] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)312,
 (uint8_t*)478,
 (uint8_t*)739,
 (uint8_t*)818,
 (uint8_t*)890,
 (uint8_t*)966,
 (uint8_t*)1047,
 (uint8_t*)1131,
 (uint8_t*)1197,
 (uint8_t*)1280,
 (uint8_t*)1386,
 (uint8_t*)1479,
 (uint8_t*)1580,
 (uint8_t*)1674,
 (uint8_t*)1737,
 (uint8_t*)1811,
 (uint8_t*)1900,
 (uint8_t*)1989,
 (uint8_t*)2062,
 (uint8_t*)2155,
 (uint8_t*)2199,
 (uint8_t*)2235,
 (uint8_t*)2276,
 (uint8_t*)2317,
 (uint8_t*)2357,
 (uint8_t*)2409,
 (uint8_t*)2461,
 (uint8_t*)2519,
 (uint8_t*)2588,
 (uint8_t*)2681,
 (uint8_t*)3496,
 (uint8_t*)4736,
 (uint8_t*)5210,
 (uint8_t*)6505,
 (uint8_t*)6565,
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
