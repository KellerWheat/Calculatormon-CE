// convpng v6.8
#include <stdint.h>
#include "PKMNSD0.h"

#include <fileioc.h>
uint8_t *PKMNSD0[80] = {
 (uint8_t*)0,
 (uint8_t*)10,
 (uint8_t*)271,
 (uint8_t*)655,
 (uint8_t*)1251,
 (uint8_t*)1620,
 (uint8_t*)2123,
 (uint8_t*)2752,
 (uint8_t*)3060,
 (uint8_t*)3557,
 (uint8_t*)4278,
 (uint8_t*)4497,
 (uint8_t*)4746,
 (uint8_t*)5332,
 (uint8_t*)5646,
 (uint8_t*)5925,
 (uint8_t*)6589,
 (uint8_t*)6954,
 (uint8_t*)7387,
 (uint8_t*)7961,
 (uint8_t*)8295,
 (uint8_t*)8807,
 (uint8_t*)9136,
 (uint8_t*)9647,
 (uint8_t*)10010,
 (uint8_t*)10643,
 (uint8_t*)10974,
 (uint8_t*)11500,
 (uint8_t*)11892,
 (uint8_t*)12455,
 (uint8_t*)12797,
 (uint8_t*)13305,
 (uint8_t*)13963,
 (uint8_t*)14335,
 (uint8_t*)14872,
 (uint8_t*)15678,
 (uint8_t*)16030,
 (uint8_t*)16508,
 (uint8_t*)17010,
 (uint8_t*)17691,
 (uint8_t*)18025,
 (uint8_t*)18450,
 (uint8_t*)18771,
 (uint8_t*)19330,
 (uint8_t*)19559,
 (uint8_t*)19984,
 (uint8_t*)20375,
 (uint8_t*)20757,
 (uint8_t*)21266,
 (uint8_t*)21652,
 (uint8_t*)22250,
 (uint8_t*)22470,
 (uint8_t*)22897,
 (uint8_t*)23344,
 (uint8_t*)23900,
 (uint8_t*)24296,
 (uint8_t*)24871,
 (uint8_t*)25290,
 (uint8_t*)25894,
 (uint8_t*)26302,
 (uint8_t*)27007,
 (uint8_t*)27322,
 (uint8_t*)27836,
 (uint8_t*)28384,
 (uint8_t*)28784,
 (uint8_t*)29404,
 (uint8_t*)30079,
 (uint8_t*)30480,
 (uint8_t*)31085,
 (uint8_t*)31906,
 (uint8_t*)32256,
 (uint8_t*)32628,
 (uint8_t*)33195,
 (uint8_t*)33531,
 (uint8_t*)34051,
 (uint8_t*)34390,
 (uint8_t*)34938,
 (uint8_t*)35460,
 (uint8_t*)35965,
 (uint8_t*)36682,
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
