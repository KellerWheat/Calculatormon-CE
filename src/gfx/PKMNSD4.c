// convpng v7.0
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[47] = {
 (uint8_t*)0,
 (uint8_t*)138,
 (uint8_t*)269,
 (uint8_t*)397,
 (uint8_t*)537,
 (uint8_t*)672,
 (uint8_t*)802,
 (uint8_t*)955,
 (uint8_t*)1104,
 (uint8_t*)1251,
 (uint8_t*)1399,
 (uint8_t*)1542,
 (uint8_t*)1688,
 (uint8_t*)1819,
 (uint8_t*)1946,
 (uint8_t*)2085,
 (uint8_t*)2219,
 (uint8_t*)2349,
 (uint8_t*)2494,
 (uint8_t*)2624,
 (uint8_t*)2754,
 (uint8_t*)2883,
 (uint8_t*)3025,
 (uint8_t*)3161,
 (uint8_t*)3295,
 (uint8_t*)3490,
 (uint8_t*)3685,
 (uint8_t*)3881,
 (uint8_t*)4080,
 (uint8_t*)4271,
 (uint8_t*)4465,
 (uint8_t*)4655,
 (uint8_t*)4849,
 (uint8_t*)4989,
 (uint8_t*)5131,
 (uint8_t*)5287,
 (uint8_t*)5436,
 (uint8_t*)5583,
 (uint8_t*)5732,
 (uint8_t*)5894,
 (uint8_t*)6046,
 (uint8_t*)6226,
 (uint8_t*)6405,
 (uint8_t*)6471,
 (uint8_t*)6610,
 (uint8_t*)14362,
 (uint8_t*)17727,
};

bool PKMNSD4_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD4", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD4[0];
    for (i = 0; i < PKMNSD4_num; i++) {
        PKMNSD4[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)PKMNSD4[43] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[44] - (unsigned int)outdoortileset_tiles_compressed[0];
    for (i = 0; i < outdoortileset_tiles_num; i++) {
        outdoortileset_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[45] - (unsigned int)indoortileset_tiles_compressed[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
