// convpng v6.8
#include <stdint.h>
#include "PKMNSD3.h"

#include <fileioc.h>
uint8_t *PKMNSD3[26] = {
 (uint8_t*)0,
 (uint8_t*)1602,
 (uint8_t*)4740,
 (uint8_t*)7046,
 (uint8_t*)10184,
 (uint8_t*)11786,
 (uint8_t*)14924,
 (uint8_t*)16526,
 (uint8_t*)19664,
 (uint8_t*)22802,
 (uint8_t*)25108,
 (uint8_t*)27414,
 (uint8_t*)30552,
 (uint8_t*)32858,
 (uint8_t*)35996,
 (uint8_t*)37598,
 (uint8_t*)40736,
 (uint8_t*)42338,
 (uint8_t*)43940,
 (uint8_t*)47078,
 (uint8_t*)50216,
 (uint8_t*)51818,
 (uint8_t*)54124,
 (uint8_t*)57262,
 (uint8_t*)59568,
 (uint8_t*)62706,
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
