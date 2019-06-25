// convpng v7.0
#include <stdint.h>
#include "PKMNSD1.h"

#include <fileioc.h>
uint8_t *PKMNSD1[72] = {
 (uint8_t*)0,
 (uint8_t*)589,
 (uint8_t*)790,
 (uint8_t*)1315,
 (uint8_t*)1862,
 (uint8_t*)2223,
 (uint8_t*)2855,
 (uint8_t*)3320,
 (uint8_t*)3949,
 (uint8_t*)4372,
 (uint8_t*)4970,
 (uint8_t*)5343,
 (uint8_t*)6005,
 (uint8_t*)6469,
 (uint8_t*)6906,
 (uint8_t*)7297,
 (uint8_t*)7925,
 (uint8_t*)8451,
 (uint8_t*)9090,
 (uint8_t*)9520,
 (uint8_t*)10092,
 (uint8_t*)10273,
 (uint8_t*)10570,
 (uint8_t*)11196,
 (uint8_t*)11774,
 (uint8_t*)12160,
 (uint8_t*)12647,
 (uint8_t*)13225,
 (uint8_t*)13643,
 (uint8_t*)14175,
 (uint8_t*)14473,
 (uint8_t*)14962,
 (uint8_t*)15616,
 (uint8_t*)16403,
 (uint8_t*)16858,
 (uint8_t*)17306,
 (uint8_t*)18005,
 (uint8_t*)18260,
 (uint8_t*)18696,
 (uint8_t*)19189,
 (uint8_t*)19844,
 (uint8_t*)20253,
 (uint8_t*)20796,
 (uint8_t*)21323,
 (uint8_t*)21946,
 (uint8_t*)22398,
 (uint8_t*)23002,
 (uint8_t*)23599,
 (uint8_t*)24194,
 (uint8_t*)24872,
 (uint8_t*)25391,
 (uint8_t*)26168,
 (uint8_t*)26684,
 (uint8_t*)26898,
 (uint8_t*)27311,
 (uint8_t*)27871,
 (uint8_t*)28404,
 (uint8_t*)28907,
 (uint8_t*)29256,
 (uint8_t*)29594,
 (uint8_t*)30128,
 (uint8_t*)30418,
 (uint8_t*)30987,
 (uint8_t*)31633,
 (uint8_t*)32164,
 (uint8_t*)32958,
 (uint8_t*)33636,
 (uint8_t*)34244,
 (uint8_t*)34571,
 (uint8_t*)35058,
 (uint8_t*)35731,
 (uint8_t*)36325,
};

bool PKMNSD1_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD1", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD1[0];
    for (i = 0; i < PKMNSD1_num; i++) {
        PKMNSD1[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
