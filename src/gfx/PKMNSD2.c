// convpng v6.8
#include <stdint.h>
#include "PKMNSD2.h"

#include <fileioc.h>
uint8_t *PKMNSD2[80] = {
 (uint8_t*)0,
 (uint8_t*)10,
 (uint8_t*)301,
 (uint8_t*)670,
 (uint8_t*)1130,
 (uint8_t*)1463,
 (uint8_t*)1862,
 (uint8_t*)2244,
 (uint8_t*)2516,
 (uint8_t*)2909,
 (uint8_t*)3329,
 (uint8_t*)3655,
 (uint8_t*)3938,
 (uint8_t*)4402,
 (uint8_t*)4700,
 (uint8_t*)5003,
 (uint8_t*)5417,
 (uint8_t*)5887,
 (uint8_t*)6199,
 (uint8_t*)6535,
 (uint8_t*)6911,
 (uint8_t*)7336,
 (uint8_t*)7688,
 (uint8_t*)7960,
 (uint8_t*)8365,
 (uint8_t*)8666,
 (uint8_t*)9040,
 (uint8_t*)9404,
 (uint8_t*)9882,
 (uint8_t*)10345,
 (uint8_t*)10723,
 (uint8_t*)11167,
 (uint8_t*)11575,
 (uint8_t*)12053,
 (uint8_t*)12477,
 (uint8_t*)13040,
 (uint8_t*)13417,
 (uint8_t*)13881,
 (uint8_t*)14264,
 (uint8_t*)14748,
 (uint8_t*)14987,
 (uint8_t*)15239,
 (uint8_t*)15654,
 (uint8_t*)15857,
 (uint8_t*)16143,
 (uint8_t*)16539,
 (uint8_t*)16904,
 (uint8_t*)17242,
 (uint8_t*)17567,
 (uint8_t*)17938,
 (uint8_t*)18443,
 (uint8_t*)18644,
 (uint8_t*)18979,
 (uint8_t*)19353,
 (uint8_t*)19774,
 (uint8_t*)20157,
 (uint8_t*)20528,
 (uint8_t*)20979,
 (uint8_t*)21371,
 (uint8_t*)21775,
 (uint8_t*)22196,
 (uint8_t*)22428,
 (uint8_t*)22774,
 (uint8_t*)23175,
 (uint8_t*)23565,
 (uint8_t*)23959,
 (uint8_t*)24453,
 (uint8_t*)24839,
 (uint8_t*)25231,
 (uint8_t*)25787,
 (uint8_t*)26097,
 (uint8_t*)26456,
 (uint8_t*)26799,
 (uint8_t*)27136,
 (uint8_t*)27518,
 (uint8_t*)27937,
 (uint8_t*)28321,
 (uint8_t*)28530,
 (uint8_t*)28954,
 (uint8_t*)29378,
};

bool PKMNSD2_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD2", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD2[0];
    for (i = 0; i < PKMNSD2_num; i++) {
        PKMNSD2[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
