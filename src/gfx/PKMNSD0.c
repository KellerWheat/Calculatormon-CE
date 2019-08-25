// convpng v7.2
#include <stdint.h>
#include "PKMNSD0.h"

#include <fileioc.h>
uint8_t *PKMNSD0[80] = {
 (uint8_t*)0,
 (uint8_t*)10,
 (uint8_t*)274,
 (uint8_t*)666,
 (uint8_t*)1280,
 (uint8_t*)1662,
 (uint8_t*)2187,
 (uint8_t*)2840,
 (uint8_t*)3155,
 (uint8_t*)3664,
 (uint8_t*)4405,
 (uint8_t*)4634,
 (uint8_t*)4888,
 (uint8_t*)5461,
 (uint8_t*)5784,
 (uint8_t*)6068,
 (uint8_t*)6767,
 (uint8_t*)7149,
 (uint8_t*)7599,
 (uint8_t*)8192,
 (uint8_t*)8539,
 (uint8_t*)9062,
 (uint8_t*)9403,
 (uint8_t*)9933,
 (uint8_t*)10309,
 (uint8_t*)10953,
 (uint8_t*)11290,
 (uint8_t*)11820,
 (uint8_t*)12226,
 (uint8_t*)12810,
 (uint8_t*)13162,
 (uint8_t*)13686,
 (uint8_t*)14360,
 (uint8_t*)14741,
 (uint8_t*)15285,
 (uint8_t*)16116,
 (uint8_t*)16474,
 (uint8_t*)16963,
 (uint8_t*)17475,
 (uint8_t*)18173,
 (uint8_t*)18517,
 (uint8_t*)18957,
 (uint8_t*)19279,
 (uint8_t*)19847,
 (uint8_t*)20080,
 (uint8_t*)20512,
 (uint8_t*)20912,
 (uint8_t*)21301,
 (uint8_t*)21817,
 (uint8_t*)22217,
 (uint8_t*)22828,
 (uint8_t*)23050,
 (uint8_t*)23488,
 (uint8_t*)23951,
 (uint8_t*)24522,
 (uint8_t*)24928,
 (uint8_t*)25521,
 (uint8_t*)25954,
 (uint8_t*)26580,
 (uint8_t*)27001,
 (uint8_t*)27730,
 (uint8_t*)28056,
 (uint8_t*)28581,
 (uint8_t*)29146,
 (uint8_t*)29555,
 (uint8_t*)30187,
 (uint8_t*)30875,
 (uint8_t*)31286,
 (uint8_t*)31899,
 (uint8_t*)32742,
 (uint8_t*)33099,
 (uint8_t*)33485,
 (uint8_t*)34072,
 (uint8_t*)34415,
 (uint8_t*)34943,
 (uint8_t*)35285,
 (uint8_t*)35840,
 (uint8_t*)36375,
 (uint8_t*)36898,
 (uint8_t*)37632,
};

bool PKMNSD0_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD0", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD0[0];
    for (i = 0; i < PKMNSD0_num; i++) {
        PKMNSD0[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
