// convpng v7.2
#include <stdint.h>
#include "PKMNSD7.h"

#include <fileioc.h>
uint8_t *PKMNSD7[44] = {
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
 (uint8_t*)8112,
 (uint8_t*)8450,
 (uint8_t*)8788,
 (uint8_t*)9126,
 (uint8_t*)9464,
 (uint8_t*)9802,
 (uint8_t*)10140,
 (uint8_t*)10478,
 (uint8_t*)10816,
 (uint8_t*)11154,
 (uint8_t*)11492,
 (uint8_t*)11830,
 (uint8_t*)12168,
 (uint8_t*)12506,
 (uint8_t*)12844,
 (uint8_t*)13182,
 (uint8_t*)13520,
 (uint8_t*)13858,
 (uint8_t*)14196,
 (uint8_t*)14534,
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
