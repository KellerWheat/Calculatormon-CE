// convpng v6.8
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[38] = {
 (uint8_t*)0,
 (uint8_t*)72,
 (uint8_t*)146,
 (uint8_t*)310,
 (uint8_t*)474,
 (uint8_t*)736,
 (uint8_t*)815,
 (uint8_t*)887,
 (uint8_t*)963,
 (uint8_t*)1044,
 (uint8_t*)1128,
 (uint8_t*)1194,
 (uint8_t*)1277,
 (uint8_t*)1383,
 (uint8_t*)1476,
 (uint8_t*)1577,
 (uint8_t*)1671,
 (uint8_t*)1734,
 (uint8_t*)1808,
 (uint8_t*)1897,
 (uint8_t*)1986,
 (uint8_t*)2059,
 (uint8_t*)2152,
 (uint8_t*)2202,
 (uint8_t*)2246,
 (uint8_t*)2292,
 (uint8_t*)2333,
 (uint8_t*)2379,
 (uint8_t*)2431,
 (uint8_t*)2483,
 (uint8_t*)2541,
 (uint8_t*)2610,
 (uint8_t*)2703,
 (uint8_t*)3518,
 (uint8_t*)4373,
 (uint8_t*)4966,
 (uint8_t*)6202,
 (uint8_t*)6262,
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
