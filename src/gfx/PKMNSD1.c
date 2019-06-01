// convpng v6.8
#include <stdint.h>
#include "PKMNSD1.h"

#include <fileioc.h>
uint8_t *PKMNSD1[73] = {
 (uint8_t*)0,
 (uint8_t*)565,
 (uint8_t*)753,
 (uint8_t*)1257,
 (uint8_t*)1783,
 (uint8_t*)2136,
 (uint8_t*)2750,
 (uint8_t*)3198,
 (uint8_t*)3803,
 (uint8_t*)4219,
 (uint8_t*)4811,
 (uint8_t*)5175,
 (uint8_t*)5829,
 (uint8_t*)6259,
 (uint8_t*)6687,
 (uint8_t*)7074,
 (uint8_t*)7693,
 (uint8_t*)8214,
 (uint8_t*)8838,
 (uint8_t*)9255,
 (uint8_t*)9817,
 (uint8_t*)9992,
 (uint8_t*)10282,
 (uint8_t*)10896,
 (uint8_t*)11459,
 (uint8_t*)11837,
 (uint8_t*)12307,
 (uint8_t*)12869,
 (uint8_t*)13276,
 (uint8_t*)13797,
 (uint8_t*)14089,
 (uint8_t*)14566,
 (uint8_t*)15208,
 (uint8_t*)15975,
 (uint8_t*)16424,
 (uint8_t*)16863,
 (uint8_t*)17541,
 (uint8_t*)17786,
 (uint8_t*)18213,
 (uint8_t*)18680,
 (uint8_t*)19322,
 (uint8_t*)19716,
 (uint8_t*)20246,
 (uint8_t*)20756,
 (uint8_t*)21363,
 (uint8_t*)21807,
 (uint8_t*)22397,
 (uint8_t*)22981,
 (uint8_t*)23560,
 (uint8_t*)24229,
 (uint8_t*)24730,
 (uint8_t*)25475,
 (uint8_t*)25978,
 (uint8_t*)26191,
 (uint8_t*)26596,
 (uint8_t*)27141,
 (uint8_t*)27652,
 (uint8_t*)28130,
 (uint8_t*)28476,
 (uint8_t*)28806,
 (uint8_t*)29325,
 (uint8_t*)29610,
 (uint8_t*)30165,
 (uint8_t*)30800,
 (uint8_t*)31315,
 (uint8_t*)32084,
 (uint8_t*)32743,
 (uint8_t*)33334,
 (uint8_t*)33653,
 (uint8_t*)34123,
 (uint8_t*)34775,
 (uint8_t*)35359,
 (uint8_t*)35696,
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
