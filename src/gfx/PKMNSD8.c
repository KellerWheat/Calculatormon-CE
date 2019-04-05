// convpng v6.8
#include <stdint.h>
#include "PKMNSD8.h"

#include <fileioc.h>
uint8_t *PKMNSD8[29] = {
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
 (uint8_t*)29978,
 (uint8_t*)32284,
 (uint8_t*)34590,
 (uint8_t*)36896,
 (uint8_t*)39202,
 (uint8_t*)41508,
 (uint8_t*)43814,
 (uint8_t*)46120,
 (uint8_t*)48426,
 (uint8_t*)50732,
 (uint8_t*)53038,
 (uint8_t*)55344,
 (uint8_t*)57650,
 (uint8_t*)59956,
 (uint8_t*)62262,
 (uint8_t*)64568,
};

bool PKMNSD8_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD8", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD8[0];
    for (i = 0; i < PKMNSD8_num; i++) {
        PKMNSD8[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
