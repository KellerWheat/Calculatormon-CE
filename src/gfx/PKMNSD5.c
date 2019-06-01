// convpng v6.8
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[38] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)311,
 (uint8_t*)475,
 (uint8_t*)734,
 (uint8_t*)813,
 (uint8_t*)885,
 (uint8_t*)961,
 (uint8_t*)1042,
 (uint8_t*)1126,
 (uint8_t*)1192,
 (uint8_t*)1275,
 (uint8_t*)1381,
 (uint8_t*)1474,
 (uint8_t*)1575,
 (uint8_t*)1669,
 (uint8_t*)1732,
 (uint8_t*)1806,
 (uint8_t*)1895,
 (uint8_t*)1984,
 (uint8_t*)2057,
 (uint8_t*)2150,
 (uint8_t*)2194,
 (uint8_t*)2230,
 (uint8_t*)2271,
 (uint8_t*)2312,
 (uint8_t*)2352,
 (uint8_t*)2404,
 (uint8_t*)2456,
 (uint8_t*)2514,
 (uint8_t*)2583,
 (uint8_t*)2677,
 (uint8_t*)3494,
 (uint8_t*)4695,
 (uint8_t*)5288,
 (uint8_t*)6482,
 (uint8_t*)6542,
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
