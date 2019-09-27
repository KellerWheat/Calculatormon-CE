// convpng v7.0
#include <stdint.h>
#include "PKMNSD8.h"

#include <fileioc.h>
uint8_t *PKMNSD8[80] = {
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
 (uint8_t*)12216,
 (uint8_t*)12554,
 (uint8_t*)12892,
 (uint8_t*)13230,
 (uint8_t*)13568,
 (uint8_t*)13906,
 (uint8_t*)14244,
 (uint8_t*)14582,
 (uint8_t*)14920,
 (uint8_t*)15258,
 (uint8_t*)15596,
 (uint8_t*)15934,
 (uint8_t*)16272,
 (uint8_t*)16610,
 (uint8_t*)16948,
 (uint8_t*)17286,
 (uint8_t*)17624,
 (uint8_t*)17962,
 (uint8_t*)18300,
 (uint8_t*)18638,
 (uint8_t*)18976,
 (uint8_t*)19314,
 (uint8_t*)19652,
 (uint8_t*)19990,
 (uint8_t*)20328,
 (uint8_t*)20666,
 (uint8_t*)21004,
 (uint8_t*)21342,
 (uint8_t*)21680,
 (uint8_t*)22018,
 (uint8_t*)22356,
 (uint8_t*)22694,
 (uint8_t*)23032,
 (uint8_t*)23370,
 (uint8_t*)23708,
 (uint8_t*)24046,
 (uint8_t*)24384,
 (uint8_t*)24722,
 (uint8_t*)25060,
 (uint8_t*)25398,
 (uint8_t*)25736,
 (uint8_t*)26074,
 (uint8_t*)26412,
 (uint8_t*)26750,
 (uint8_t*)27088,
 (uint8_t*)27426,
 (uint8_t*)27764,
 (uint8_t*)28102,
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
