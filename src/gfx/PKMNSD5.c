// convpng v6.8
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[38] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)326,
 (uint8_t*)504,
 (uint8_t*)780,
 (uint8_t*)859,
 (uint8_t*)931,
 (uint8_t*)1007,
 (uint8_t*)1088,
 (uint8_t*)1172,
 (uint8_t*)1238,
 (uint8_t*)1321,
 (uint8_t*)1427,
 (uint8_t*)1520,
 (uint8_t*)1621,
 (uint8_t*)1715,
 (uint8_t*)1778,
 (uint8_t*)1852,
 (uint8_t*)1941,
 (uint8_t*)2030,
 (uint8_t*)2103,
 (uint8_t*)2196,
 (uint8_t*)2240,
 (uint8_t*)2276,
 (uint8_t*)2317,
 (uint8_t*)2358,
 (uint8_t*)2398,
 (uint8_t*)2450,
 (uint8_t*)2502,
 (uint8_t*)2560,
 (uint8_t*)2629,
 (uint8_t*)2723,
 (uint8_t*)3540,
 (uint8_t*)4817,
 (uint8_t*)5410,
 (uint8_t*)6785,
 (uint8_t*)6845,
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
