// convpng v7.2
#include <stdint.h>
#include "PKMNSD8.h"

#include <fileioc.h>
uint8_t *PKMNSD8[32] = {
 (uint8_t*)0,
 (uint8_t*)322,
 (uint8_t*)644,
 (uint8_t*)966,
 (uint8_t*)1288,
 (uint8_t*)1610,
 (uint8_t*)1932,
 (uint8_t*)2254,
 (uint8_t*)2576,
 (uint8_t*)2898,
 (uint8_t*)3220,
 (uint8_t*)3542,
 (uint8_t*)3864,
 (uint8_t*)4186,
 (uint8_t*)4508,
 (uint8_t*)4830,
 (uint8_t*)5152,
 (uint8_t*)5474,
 (uint8_t*)5796,
 (uint8_t*)6118,
 (uint8_t*)6440,
 (uint8_t*)6762,
 (uint8_t*)7084,
 (uint8_t*)7406,
 (uint8_t*)7728,
 (uint8_t*)8302,
 (uint8_t*)8876,
 (uint8_t*)9450,
 (uint8_t*)10024,
 (uint8_t*)10546,
 (uint8_t*)11120,
 (uint8_t*)11642,
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
