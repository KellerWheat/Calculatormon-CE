// convpng v7.0
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[34] = {
 (uint8_t*)0,
 (uint8_t*)79,
 (uint8_t*)151,
 (uint8_t*)227,
 (uint8_t*)308,
 (uint8_t*)392,
 (uint8_t*)458,
 (uint8_t*)541,
 (uint8_t*)647,
 (uint8_t*)740,
 (uint8_t*)841,
 (uint8_t*)935,
 (uint8_t*)998,
 (uint8_t*)1072,
 (uint8_t*)1161,
 (uint8_t*)1250,
 (uint8_t*)1323,
 (uint8_t*)1416,
 (uint8_t*)1460,
 (uint8_t*)1501,
 (uint8_t*)1537,
 (uint8_t*)1578,
 (uint8_t*)1618,
 (uint8_t*)1670,
 (uint8_t*)1722,
 (uint8_t*)1780,
 (uint8_t*)1849,
 (uint8_t*)1943,
 (uint8_t*)2759,
 (uint8_t*)3934,
 (uint8_t*)4408,
 (uint8_t*)5207,
 (uint8_t*)6331,
 (uint8_t*)6391,
};

bool PKMNSD5_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD5", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD5[0];
    for (i = 0; i < PKMNSD5_num; i++) {
        PKMNSD5[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
