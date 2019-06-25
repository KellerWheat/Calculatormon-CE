// convpng v7.0
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[39] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)306,
 (uint8_t*)466,
 (uint8_t*)715,
 (uint8_t*)794,
 (uint8_t*)866,
 (uint8_t*)942,
 (uint8_t*)1023,
 (uint8_t*)1107,
 (uint8_t*)1173,
 (uint8_t*)1256,
 (uint8_t*)1362,
 (uint8_t*)1455,
 (uint8_t*)1556,
 (uint8_t*)1650,
 (uint8_t*)1713,
 (uint8_t*)1787,
 (uint8_t*)1876,
 (uint8_t*)1965,
 (uint8_t*)2038,
 (uint8_t*)2131,
 (uint8_t*)2175,
 (uint8_t*)2211,
 (uint8_t*)2252,
 (uint8_t*)2293,
 (uint8_t*)2333,
 (uint8_t*)2385,
 (uint8_t*)2437,
 (uint8_t*)2495,
 (uint8_t*)2564,
 (uint8_t*)2658,
 (uint8_t*)3476,
 (uint8_t*)4677,
 (uint8_t*)5151,
 (uint8_t*)5951,
 (uint8_t*)7087,
 (uint8_t*)7147,
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
