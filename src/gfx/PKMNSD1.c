// convpng v6.8
#include <stdint.h>
#include "PKMNSD1.h"

#include <fileioc.h>
uint8_t *PKMNSD1[73] = {
 (uint8_t*)0,
 (uint8_t*)565,
 (uint8_t*)753,
 (uint8_t*)1257,
 (uint8_t*)1783,
 (uint8_t*)2136,
 (uint8_t*)2750,
 (uint8_t*)3198,
 (uint8_t*)3803,
 (uint8_t*)4218,
 (uint8_t*)4810,
 (uint8_t*)5174,
 (uint8_t*)5828,
 (uint8_t*)6258,
 (uint8_t*)6686,
 (uint8_t*)7073,
 (uint8_t*)7692,
 (uint8_t*)8213,
 (uint8_t*)8837,
 (uint8_t*)9254,
 (uint8_t*)9816,
 (uint8_t*)9991,
 (uint8_t*)10281,
 (uint8_t*)10895,
 (uint8_t*)11458,
 (uint8_t*)11836,
 (uint8_t*)12306,
 (uint8_t*)12868,
 (uint8_t*)13275,
 (uint8_t*)13796,
 (uint8_t*)14088,
 (uint8_t*)14565,
 (uint8_t*)15207,
 (uint8_t*)15974,
 (uint8_t*)16423,
 (uint8_t*)16862,
 (uint8_t*)17540,
 (uint8_t*)17785,
 (uint8_t*)18212,
 (uint8_t*)18679,
 (uint8_t*)19321,
 (uint8_t*)19715,
 (uint8_t*)20245,
 (uint8_t*)20755,
 (uint8_t*)21362,
 (uint8_t*)21806,
 (uint8_t*)22396,
 (uint8_t*)22980,
 (uint8_t*)23559,
 (uint8_t*)24228,
 (uint8_t*)24729,
 (uint8_t*)25474,
 (uint8_t*)25977,
 (uint8_t*)26190,
 (uint8_t*)26595,
 (uint8_t*)27140,
 (uint8_t*)27651,
 (uint8_t*)28129,
 (uint8_t*)28475,
 (uint8_t*)28805,
 (uint8_t*)29324,
 (uint8_t*)29609,
 (uint8_t*)30164,
 (uint8_t*)30799,
 (uint8_t*)31314,
 (uint8_t*)32083,
 (uint8_t*)32742,
 (uint8_t*)33333,
 (uint8_t*)33652,
 (uint8_t*)34122,
 (uint8_t*)34774,
 (uint8_t*)35358,
 (uint8_t*)35695,
};

bool PKMNSD1_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD1", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD1[0];
    for (i = 0; i < PKMNSD1_num; i++) {
        PKMNSD1[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
