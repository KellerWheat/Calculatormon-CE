// convpng v7.0
#include <stdint.h>
#include "PKMNSD2.h"

#include <fileioc.h>
uint8_t *PKMNSD2[80] = {
 (uint8_t*)0,
 (uint8_t*)10,
 (uint8_t*)307,
 (uint8_t*)684,
 (uint8_t*)1157,
 (uint8_t*)1499,
 (uint8_t*)1902,
 (uint8_t*)2291,
 (uint8_t*)2570,
 (uint8_t*)2973,
 (uint8_t*)3410,
 (uint8_t*)3744,
 (uint8_t*)4034,
 (uint8_t*)4510,
 (uint8_t*)4812,
 (uint8_t*)5118,
 (uint8_t*)5542,
 (uint8_t*)6022,
 (uint8_t*)6342,
 (uint8_t*)6691,
 (uint8_t*)7078,
 (uint8_t*)7508,
 (uint8_t*)7874,
 (uint8_t*)8151,
 (uint8_t*)8563,
 (uint8_t*)8866,
 (uint8_t*)9246,
 (uint8_t*)9611,
 (uint8_t*)10099,
 (uint8_t*)10571,
 (uint8_t*)10957,
 (uint8_t*)11415,
 (uint8_t*)11834,
 (uint8_t*)12319,
 (uint8_t*)12745,
 (uint8_t*)13322,
 (uint8_t*)13707,
 (uint8_t*)14178,
 (uint8_t*)14566,
 (uint8_t*)15061,
 (uint8_t*)15306,
 (uint8_t*)15562,
 (uint8_t*)15979,
 (uint8_t*)16189,
 (uint8_t*)16479,
 (uint8_t*)16883,
 (uint8_t*)17258,
 (uint8_t*)17609,
 (uint8_t*)17938,
 (uint8_t*)18321,
 (uint8_t*)18830,
 (uint8_t*)19034,
 (uint8_t*)19371,
 (uint8_t*)19754,
 (uint8_t*)20185,
 (uint8_t*)20573,
 (uint8_t*)20951,
 (uint8_t*)21410,
 (uint8_t*)21815,
 (uint8_t*)22233,
 (uint8_t*)22673,
 (uint8_t*)22910,
 (uint8_t*)23267,
 (uint8_t*)23674,
 (uint8_t*)24072,
 (uint8_t*)24471,
 (uint8_t*)24976,
 (uint8_t*)25373,
 (uint8_t*)25774,
 (uint8_t*)26343,
 (uint8_t*)26656,
 (uint8_t*)27026,
 (uint8_t*)27379,
 (uint8_t*)27720,
 (uint8_t*)28109,
 (uint8_t*)28529,
 (uint8_t*)28919,
 (uint8_t*)29131,
 (uint8_t*)29560,
 (uint8_t*)29991,
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
