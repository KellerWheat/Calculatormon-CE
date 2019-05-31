// convpng v6.8
#include <stdint.h>
#include "PKMNSD0.h"

#include <fileioc.h>
uint8_t *PKMNSD0[80] = {
 (uint8_t*)0,
 (uint8_t*)10,
 (uint8_t*)275,
 (uint8_t*)667,
 (uint8_t*)1281,
 (uint8_t*)1665,
 (uint8_t*)2189,
 (uint8_t*)2842,
 (uint8_t*)3158,
 (uint8_t*)3667,
 (uint8_t*)4408,
 (uint8_t*)4638,
 (uint8_t*)4892,
 (uint8_t*)5465,
 (uint8_t*)5788,
 (uint8_t*)6072,
 (uint8_t*)6771,
 (uint8_t*)7153,
 (uint8_t*)7603,
 (uint8_t*)8196,
 (uint8_t*)8544,
 (uint8_t*)9070,
 (uint8_t*)9411,
 (uint8_t*)9941,
 (uint8_t*)10317,
 (uint8_t*)10961,
 (uint8_t*)11299,
 (uint8_t*)11829,
 (uint8_t*)12237,
 (uint8_t*)12821,
 (uint8_t*)13173,
 (uint8_t*)13697,
 (uint8_t*)14371,
 (uint8_t*)14752,
 (uint8_t*)15303,
 (uint8_t*)16132,
 (uint8_t*)16492,
 (uint8_t*)16981,
 (uint8_t*)17494,
 (uint8_t*)18192,
 (uint8_t*)18536,
 (uint8_t*)18978,
 (uint8_t*)19303,
 (uint8_t*)19876,
 (uint8_t*)20109,
 (uint8_t*)20541,
 (uint8_t*)20941,
 (uint8_t*)21331,
 (uint8_t*)21847,
 (uint8_t*)22249,
 (uint8_t*)22860,
 (uint8_t*)23083,
 (uint8_t*)23521,
 (uint8_t*)23986,
 (uint8_t*)24557,
 (uint8_t*)24967,
 (uint8_t*)25577,
 (uint8_t*)26016,
 (uint8_t*)26642,
 (uint8_t*)27067,
 (uint8_t*)27796,
 (uint8_t*)28122,
 (uint8_t*)28649,
 (uint8_t*)29214,
 (uint8_t*)29624,
 (uint8_t*)30257,
 (uint8_t*)30945,
 (uint8_t*)31360,
 (uint8_t*)31973,
 (uint8_t*)32816,
 (uint8_t*)33175,
 (uint8_t*)33561,
 (uint8_t*)34148,
 (uint8_t*)34493,
 (uint8_t*)35026,
 (uint8_t*)35368,
 (uint8_t*)35928,
 (uint8_t*)36464,
 (uint8_t*)36988,
 (uint8_t*)37723,
};

bool PKMNSD0_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD0", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD0[0];
    for (i = 0; i < PKMNSD0_num; i++) {
        PKMNSD0[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
