// convpng v7.0
#include <stdint.h>
#include "PKMNSD7.h"

#include <fileioc.h>
uint8_t *PKMNSD7[24] = {
 (uint8_t*)0,
 (uint8_t*)338,
 (uint8_t*)676,
 (uint8_t*)1014,
 (uint8_t*)1352,
 (uint8_t*)1690,
 (uint8_t*)2028,
 (uint8_t*)2366,
 (uint8_t*)2704,
 (uint8_t*)3042,
 (uint8_t*)3380,
 (uint8_t*)3718,
 (uint8_t*)4056,
 (uint8_t*)4394,
 (uint8_t*)4732,
 (uint8_t*)5070,
 (uint8_t*)5408,
 (uint8_t*)5746,
 (uint8_t*)6084,
 (uint8_t*)6422,
 (uint8_t*)6760,
 (uint8_t*)7098,
 (uint8_t*)7436,
 (uint8_t*)7774,
};

bool PKMNSD7_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD7", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD7[0];
    for (i = 0; i < PKMNSD7_num; i++) {
        PKMNSD7[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
