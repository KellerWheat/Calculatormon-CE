// convpng v6.8
#include <stdint.h>
#include "PKMNSD1.h"

#include <fileioc.h>
uint8_t *PKMNSD1[73] = {
 (uint8_t*)0,
 (uint8_t*)565,
 (uint8_t*)753,
 (uint8_t*)1257,
 (uint8_t*)1784,
 (uint8_t*)2141,
 (uint8_t*)2755,
 (uint8_t*)3204,
 (uint8_t*)3809,
 (uint8_t*)4227,
 (uint8_t*)4819,
 (uint8_t*)5183,
 (uint8_t*)5837,
 (uint8_t*)6267,
 (uint8_t*)6697,
 (uint8_t*)7086,
 (uint8_t*)7705,
 (uint8_t*)8227,
 (uint8_t*)8851,
 (uint8_t*)9270,
 (uint8_t*)9832,
 (uint8_t*)10007,
 (uint8_t*)10297,
 (uint8_t*)10910,
 (uint8_t*)11473,
 (uint8_t*)11851,
 (uint8_t*)12323,
 (uint8_t*)12885,
 (uint8_t*)13296,
 (uint8_t*)13817,
 (uint8_t*)14109,
 (uint8_t*)14586,
 (uint8_t*)15228,
 (uint8_t*)15995,
 (uint8_t*)16445,
 (uint8_t*)16884,
 (uint8_t*)17562,
 (uint8_t*)17808,
 (uint8_t*)18235,
 (uint8_t*)18702,
 (uint8_t*)19344,
 (uint8_t*)19738,
 (uint8_t*)20268,
 (uint8_t*)20782,
 (uint8_t*)21389,
 (uint8_t*)21833,
 (uint8_t*)22426,
 (uint8_t*)23012,
 (uint8_t*)23591,
 (uint8_t*)24260,
 (uint8_t*)24762,
 (uint8_t*)25507,
 (uint8_t*)26010,
 (uint8_t*)26224,
 (uint8_t*)26633,
 (uint8_t*)27178,
 (uint8_t*)27691,
 (uint8_t*)28169,
 (uint8_t*)28515,
 (uint8_t*)28847,
 (uint8_t*)29368,
 (uint8_t*)29653,
 (uint8_t*)30212,
 (uint8_t*)30847,
 (uint8_t*)31362,
 (uint8_t*)32131,
 (uint8_t*)32790,
 (uint8_t*)33381,
 (uint8_t*)33700,
 (uint8_t*)34172,
 (uint8_t*)34824,
 (uint8_t*)35408,
 (uint8_t*)35745,
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
