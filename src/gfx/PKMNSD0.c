// convpng v6.8
#include <stdint.h>
#include "PKMNSD0.h"

#include <fileioc.h>
uint8_t *PKMNSD0[31] = {
 (uint8_t*)0,
 (uint8_t*)3,
 (uint8_t*)1605,
 (uint8_t*)3911,
 (uint8_t*)7049,
 (uint8_t*)8651,
 (uint8_t*)10957,
 (uint8_t*)14095,
 (uint8_t*)15697,
 (uint8_t*)18003,
 (uint8_t*)21141,
 (uint8_t*)22743,
 (uint8_t*)24345,
 (uint8_t*)27483,
 (uint8_t*)29085,
 (uint8_t*)30687,
 (uint8_t*)33825,
 (uint8_t*)35427,
 (uint8_t*)37733,
 (uint8_t*)40871,
 (uint8_t*)42473,
 (uint8_t*)44779,
 (uint8_t*)46381,
 (uint8_t*)49519,
 (uint8_t*)51121,
 (uint8_t*)54259,
 (uint8_t*)55861,
 (uint8_t*)58999,
 (uint8_t*)60601,
 (uint8_t*)62907,
 (uint8_t*)64509,
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
