// convpng v6.8
#include <stdint.h>
#include "PKMNSD1.h"

#include <fileioc.h>
uint8_t *PKMNSD1[28] = {
 (uint8_t*)0,
 (uint8_t*)2306,
 (uint8_t*)5444,
 (uint8_t*)7046,
 (uint8_t*)9352,
 (uint8_t*)12490,
 (uint8_t*)14092,
 (uint8_t*)16398,
 (uint8_t*)18704,
 (uint8_t*)21842,
 (uint8_t*)23444,
 (uint8_t*)25750,
 (uint8_t*)27352,
 (uint8_t*)30490,
 (uint8_t*)32092,
 (uint8_t*)34398,
 (uint8_t*)37536,
 (uint8_t*)39138,
 (uint8_t*)42276,
 (uint8_t*)43878,
 (uint8_t*)47016,
 (uint8_t*)48618,
 (uint8_t*)51756,
 (uint8_t*)53358,
 (uint8_t*)56496,
 (uint8_t*)58098,
 (uint8_t*)61236,
 (uint8_t*)62838,
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
