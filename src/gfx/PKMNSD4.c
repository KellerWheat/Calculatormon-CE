// convpng v6.8
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[24] = {
 (uint8_t*)0,
 (uint8_t*)2306,
 (uint8_t*)5444,
 (uint8_t*)8582,
 (uint8_t*)11720,
 (uint8_t*)14026,
 (uint8_t*)16332,
 (uint8_t*)19470,
 (uint8_t*)21072,
 (uint8_t*)23378,
 (uint8_t*)25684,
 (uint8_t*)28822,
 (uint8_t*)31128,
 (uint8_t*)33434,
 (uint8_t*)35740,
 (uint8_t*)38878,
 (uint8_t*)41184,
 (uint8_t*)43490,
 (uint8_t*)45796,
 (uint8_t*)48934,
 (uint8_t*)52072,
 (uint8_t*)54378,
 (uint8_t*)57516,
 (uint8_t*)60654,
};

bool PKMNSD4_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD4", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD4[0];
    for (i = 0; i < PKMNSD4_num; i++) {
        PKMNSD4[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
