// convpng v6.8
#include <stdint.h>
#include "PKMNSD5.h"

#include <fileioc.h>
uint8_t *PKMNSD5[21] = {
 (uint8_t*)0,
 (uint8_t*)1602,
 (uint8_t*)3204,
 (uint8_t*)5510,
 (uint8_t*)7816,
 (uint8_t*)10122,
 (uint8_t*)12428,
 (uint8_t*)14030,
 (uint8_t*)16336,
 (uint8_t*)17938,
 (uint8_t*)20244,
 (uint8_t*)23382,
 (uint8_t*)26520,
 (uint8_t*)29658,
 (uint8_t*)32796,
 (uint8_t*)35934,
 (uint8_t*)37536,
 (uint8_t*)39842,
 (uint8_t*)42980,
 (uint8_t*)46118,
 (uint8_t*)47720,
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
