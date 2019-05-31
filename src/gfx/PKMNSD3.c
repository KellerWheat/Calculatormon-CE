// convpng v6.8
#include <stdint.h>
#include "PKMNSD3.h"

#include <fileioc.h>
uint8_t *PKMNSD3[73] = {
 (uint8_t*)0,
 (uint8_t*)367,
 (uint8_t*)573,
 (uint8_t*)1044,
 (uint8_t*)1479,
 (uint8_t*)1890,
 (uint8_t*)2412,
 (uint8_t*)2742,
 (uint8_t*)3017,
 (uint8_t*)3341,
 (uint8_t*)3705,
 (uint8_t*)4139,
 (uint8_t*)4668,
 (uint8_t*)4909,
 (uint8_t*)5230,
 (uint8_t*)5567,
 (uint8_t*)5991,
 (uint8_t*)6279,
 (uint8_t*)6725,
 (uint8_t*)7191,
 (uint8_t*)7557,
 (uint8_t*)7800,
 (uint8_t*)8035,
 (uint8_t*)8411,
 (uint8_t*)8907,
 (uint8_t*)9335,
 (uint8_t*)9738,
 (uint8_t*)10064,
 (uint8_t*)10443,
 (uint8_t*)10778,
 (uint8_t*)11189,
 (uint8_t*)11558,
 (uint8_t*)11921,
 (uint8_t*)12408,
 (uint8_t*)12743,
 (uint8_t*)12984,
 (uint8_t*)13488,
 (uint8_t*)13770,
 (uint8_t*)14232,
 (uint8_t*)14680,
 (uint8_t*)15105,
 (uint8_t*)15422,
 (uint8_t*)15707,
 (uint8_t*)16012,
 (uint8_t*)16407,
 (uint8_t*)16618,
 (uint8_t*)17035,
 (uint8_t*)17473,
 (uint8_t*)17955,
 (uint8_t*)18196,
 (uint8_t*)18627,
 (uint8_t*)19122,
 (uint8_t*)19491,
 (uint8_t*)19733,
 (uint8_t*)20152,
 (uint8_t*)20492,
 (uint8_t*)20841,
 (uint8_t*)21255,
 (uint8_t*)21561,
 (uint8_t*)21969,
 (uint8_t*)22451,
 (uint8_t*)22762,
 (uint8_t*)23191,
 (uint8_t*)23538,
 (uint8_t*)23736,
 (uint8_t*)24055,
 (uint8_t*)24416,
 (uint8_t*)24814,
 (uint8_t*)25108,
 (uint8_t*)25483,
 (uint8_t*)25810,
 (uint8_t*)26223,
 (uint8_t*)26588,
};

bool PKMNSD3_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD3", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD3[0];
    for (i = 0; i < PKMNSD3_num; i++) {
        PKMNSD3[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
