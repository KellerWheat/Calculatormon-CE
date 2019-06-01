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
 (uint8_t*)18628,
 (uint8_t*)19123,
 (uint8_t*)19492,
 (uint8_t*)19734,
 (uint8_t*)20153,
 (uint8_t*)20493,
 (uint8_t*)20842,
 (uint8_t*)21256,
 (uint8_t*)21562,
 (uint8_t*)21970,
 (uint8_t*)22452,
 (uint8_t*)22763,
 (uint8_t*)23192,
 (uint8_t*)23539,
 (uint8_t*)23737,
 (uint8_t*)24056,
 (uint8_t*)24417,
 (uint8_t*)24815,
 (uint8_t*)25109,
 (uint8_t*)25484,
 (uint8_t*)25811,
 (uint8_t*)26224,
 (uint8_t*)26589,
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
