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
 (uint8_t*)1861,
 (uint8_t*)2243,
 (uint8_t*)2515,
 (uint8_t*)2908,
 (uint8_t*)3328,
 (uint8_t*)3654,
 (uint8_t*)3937,
 (uint8_t*)4401,
 (uint8_t*)4699,
 (uint8_t*)5002,
 (uint8_t*)5416,
 (uint8_t*)5886,
 (uint8_t*)6198,
 (uint8_t*)6534,
 (uint8_t*)6910,
 (uint8_t*)7335,
 (uint8_t*)7687,
 (uint8_t*)7959,
 (uint8_t*)8364,
 (uint8_t*)8665,
 (uint8_t*)9039,
 (uint8_t*)9403,
 (uint8_t*)9881,
 (uint8_t*)10344,
 (uint8_t*)10722,
 (uint8_t*)11166,
 (uint8_t*)11574,
 (uint8_t*)12052,
 (uint8_t*)12476,
 (uint8_t*)13039,
 (uint8_t*)13416,
 (uint8_t*)13880,
 (uint8_t*)14263,
 (uint8_t*)14747,
 (uint8_t*)14986,
 (uint8_t*)15238,
 (uint8_t*)15653,
 (uint8_t*)15856,
 (uint8_t*)16142,
 (uint8_t*)16538,
 (uint8_t*)16903,
 (uint8_t*)17241,
 (uint8_t*)17566,
 (uint8_t*)17937,
 (uint8_t*)18442,
 (uint8_t*)18643,
 (uint8_t*)18978,
 (uint8_t*)19352,
 (uint8_t*)19773,
 (uint8_t*)20156,
 (uint8_t*)20527,
 (uint8_t*)20978,
 (uint8_t*)21370,
 (uint8_t*)21774,
 (uint8_t*)22195,
 (uint8_t*)22427,
 (uint8_t*)22773,
 (uint8_t*)23174,
 (uint8_t*)23564,
 (uint8_t*)23958,
 (uint8_t*)24452,
 (uint8_t*)24838,
 (uint8_t*)25230,
 (uint8_t*)25786,
 (uint8_t*)26096,
 (uint8_t*)26455,
 (uint8_t*)26798,
 (uint8_t*)27135,
 (uint8_t*)27517,
 (uint8_t*)27936,
 (uint8_t*)28320,
 (uint8_t*)28529,
 (uint8_t*)28953,
 (uint8_t*)29377,
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
