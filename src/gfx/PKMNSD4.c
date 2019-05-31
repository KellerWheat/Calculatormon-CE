// convpng v6.8
#include <stdint.h>
#include "PKMNSD4.h"

#include <fileioc.h>
uint8_t *PKMNSD4[9] = {
 (uint8_t*)0,
 (uint8_t*)6988,
 (uint8_t*)10353,
 (uint8_t*)12220,
 (uint8_t*)12865,
 (uint8_t*)13004,
 (uint8_t*)13184,
 (uint8_t*)13363,
 (uint8_t*)13429,
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

    data = (unsigned int)PKMNSD4[0] - (unsigned int)outdoortileset_tiles_compressed[0];
    for (i = 0; i < outdoortileset_tiles_num; i++) {
        outdoortileset_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[1] - (unsigned int)indoortileset_tiles_compressed[0];
    for (i = 0; i < indoortileset_tiles_num; i++) {
        indoortileset_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[2] - (unsigned int)player_tiles_compressed[0];
    for (i = 0; i < player_tiles_num; i++) {
        player_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[3] - (unsigned int)enemy_tiles_compressed[0];
    for (i = 0; i < enemy_tiles_num; i++) {
        enemy_tiles_compressed[i] += data;
    }

    data = (unsigned int)PKMNSD4[4] - (unsigned int)pokeball_tiles_compressed[0];
    for (i = 0; i < pokeball_tiles_num; i++) {
        pokeball_tiles_compressed[i] += data;
    }

    return (bool)appvar;
}
