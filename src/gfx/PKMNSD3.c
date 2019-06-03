// convpng v6.8
#include <stdint.h>
#include "PKMNSD3.h"

#include <fileioc.h>
uint8_t *PKMNSD3[73] = {
 (uint8_t*)0,
 (uint8_t*)373,
 (uint8_t*)586,
 (uint8_t*)1084,
 (uint8_t*)1534,
 (uint8_t*)1951,
 (uint8_t*)2482,
 (uint8_t*)2822,
 (uint8_t*)3102,
 (uint8_t*)3433,
 (uint8_t*)3801,
 (uint8_t*)4243,
 (uint8_t*)4778,
 (uint8_t*)5038,
 (uint8_t*)5364,
 (uint8_t*)5706,
 (uint8_t*)6137,
 (uint8_t*)6431,
 (uint8_t*)6889,
 (uint8_t*)7363,
 (uint8_t*)7734,
 (uint8_t*)7982,
 (uint8_t*)8223,
 (uint8_t*)8604,
 (uint8_t*)9116,
 (uint8_t*)9551,
 (uint8_t*)9967,
 (uint8_t*)10299,
 (uint8_t*)10684,
 (uint8_t*)11024,
 (uint8_t*)11457,
 (uint8_t*)11836,
 (uint8_t*)12205,
 (uint8_t*)12704,
 (uint8_t*)13044,
 (uint8_t*)13296,
 (uint8_t*)13807,
 (uint8_t*)14095,
 (uint8_t*)14566,
 (uint8_t*)15028,
 (uint8_t*)15462,
 (uint8_t*)15784,
 (uint8_t*)16074,
 (uint8_t*)16389,
 (uint8_t*)16797,
 (uint8_t*)17020,
 (uint8_t*)17439,
 (uint8_t*)17887,
 (uint8_t*)18378,
 (uint8_t*)18622,
 (uint8_t*)19068,
 (uint8_t*)19578,
 (uint8_t*)19966,
 (uint8_t*)20213,
 (uint8_t*)20641,
 (uint8_t*)20985,
 (uint8_t*)21352,
 (uint8_t*)21787,
 (uint8_t*)22096,
 (uint8_t*)22511,
 (uint8_t*)23004,
 (uint8_t*)23321,
 (uint8_t*)23756,
 (uint8_t*)24108,
 (uint8_t*)24309,
 (uint8_t*)24637,
 (uint8_t*)25007,
 (uint8_t*)25410,
 (uint8_t*)25713,
 (uint8_t*)26094,
 (uint8_t*)26429,
 (uint8_t*)26855,
 (uint8_t*)27227,
};

bool PKMNSD3_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("PKMNSD3", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)PKMNSD3[0];
    for (i = 0; i < PKMNSD3_num; i++) {
        PKMNSD3[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
