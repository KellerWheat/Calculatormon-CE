// convpng v6.8
#include <stdint.h>
#include "PKMNSD11.h"

#include <fileioc.h>
uint8_t *PKMNSD11[13] = {
 (uint8_t*)0,
 (uint8_t*)2306,
 (uint8_t*)4612,
 (uint8_t*)6918,
 (uint8_t*)9224,
 (uint8_t*)11530,
 (uint8_t*)13836,
 (uint8_t*)16142,
 (uint8_t*)18448,
 (uint8_t*)20754,
 (uint8_t*)23060,
 (uint8_t*)25366,
 (uint8_t*)27672,
};

bool PKMNSD11_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD11", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD11[0];
    for (i = 0; i < PKMNSD11_num; i++) {
        PKMNSD11[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
