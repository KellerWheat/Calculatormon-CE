#include "data.h";
#include "text.h";
#include <fileioc.h>

void LoadText(int index) {
	ti_var_t textAV = ti_Open("PKMNTD", "r");
	ti_Seek(256*index, 0, textAV);
	ti_Read(&loadedText, 256, 1, textAV);
	ti_CloseAll();
}
void LoadDesc(int index) {
	ti_var_t textAV = ti_Open("PKMNTDM", "r");
	ti_Seek(78 * index, 0, textAV);
	ti_Read(&loadedText, 78, 1, textAV);
	ti_CloseAll();
}
char loadedText[256];

/* Elemental damage multipliers */
uint8_t elements[18][18] = {
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},//None - 0
	{2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,2,2,1},//Normal - 1
	{2,2,1,1,2,4,4,2,2,2,2,2,4,1,2,1,2,4},//Fire - 2
	{2,2,4,1,2,1,2,2,2,4,2,2,2,4,2,1,2,2},//Water - 3
	{2,2,2,4,1,1,2,2,2,0,4,2,2,2,2,1,2,2},//Electric - 4
	{2,2,1,4,2,1,2,2,1,4,1,2,1,4,2,1,2,1},//Grass - 5
	{2,2,1,1,2,4,1,2,2,4,4,2,2,2,2,4,2,1},//Ice - 6
	{2,4,2,2,2,2,4,2,1,2,1,1,1,4,0,2,4,4},//Fighting - 7
	{2,2,2,2,2,4,2,2,1,1,2,2,2,1,1,2,2,0},//Poison - 8
	{2,2,4,2,4,1,2,2,4,2,0,2,1,4,2,2,2,4},//Ground - 9
	{2,2,2,2,1,4,2,4,2,2,2,2,4,1,2,2,2,1},//Flying - 10
	{2,2,2,2,2,2,2,4,4,2,2,1,2,2,2,2,0,1},//Psychic - 11
	{2,2,1,2,2,4,2,1,1,2,1,4,2,2,1,2,4,1},//Bug - 12
	{2,2,4,2,2,2,4,1,2,1,4,2,4,2,2,2,2,1},//Rock - 13
	{2,0,2,2,2,2,2,2,2,2,2,4,2,2,4,2,1,1},//Ghost - 14
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,1},//Dragon - 15
	{2,2,2,2,2,2,2,1,2,2,2,4,2,2,4,2,1,1},//Dark - 16
	{2,2,1,1,1,2,4,2,2,2,2,2,2,4,2,2,2,1},//Steel - 17
};

/*Slow, Medium Slow, Medium Fast, Fast*/
int xpPerLevel[4][101] = {
{ 0,1,10,33,80,156,270,428,640,911,1250,1663,2160,2746,3430,4218,5120,6141,7290,8573,10000,11576,13310,15208,17280,19531,21970,24603,27440,30486,33750,37238,40960,44921,49130,53593,58320,63316,68590,74148,80000,86151,92610,99383,106480,113906,121670,129778,138240,147061,156250,165813,175760,186096,196830,207968,219520,231491,243890,256723,270000,283726,297910,312558,327680,343281,359370,375953,393040,410636,428750,447388,466560,486271,506530,527343,548720,570666,593190,616298,640000,664301,689210,714733,740880,767656,795070,823128,851840,881211,911250,941963,973360,1005446,1038230,1071718,1105920,1140841,1176490,1212873,1250000, },
{ 0,1,9,57,96,135,179,236,314,419,560,742,973,1261,1612,2035,2535,3120,3798,4575,5460,6458,7577,8825,10208,11735,13411,15244,17242,19411,21760,24294,27021,29949,33084,36435,40007,43808,47846,52127,56660,61450,66505,71833,77440,83335,89523,96012,102810,109923,117360,125126,133229,141677,150476,159635,169159,179056,189334,199999,211060,222522,234393,246681,259392,272535,286115,300140,314618,329555,344960,360838,377197,394045,411388,429235,447591,466464,485862,505791,526260,547274,568841,590969,613664,636935,660787,685228,710266,735907,762160,789030,816525,844653,873420,902835,932903,963632,995030,1027103,1059860, },
{ 0,1,8,27,64,125,216,343,512,729,1000,1331,1728,2197,2744,3375,4096,4913,5832,6859,8000,9261,10648,12167,13824,15625,17576,19683,21952,24389,27000,29791,32768,35937,39304,42875,46656,50653,54872,59319,64000,68921,74088,79507,85184,91125,97336,103823,110592,117649,125000,132651,140608,148877,157464,166375,175616,185193,195112,205379,216000,226981,238328,250047,262144,274625,287496,300763,314432,328509,343000,357911,373248,389017,405224,421875,438976,456533,474552,493039,512000,531441,551368,571787,592704,614125,636056,658503,681472,704969,729000,753571,778688,804357,830584,857375,884736,912673,941192,970299,1000000, },
{ 0,1,6,21,51,100,172,274,409,583,800,1064,1382,1757,2195,2700,3276,3930,4665,5487,6400,7408,8518,9733,11059,12500,14060,15746,17561,19511,21600,23832,26214,28749,31443,34300,37324,40522,43897,47455,51200,55136,59270,63605,68147,72900,77868,83058,88473,94119,100000,106120,112486,119101,125971,133100,140492,148154,156089,164303,172800,181584,190662,200037,209715,219700,229996,240610,251545,262807,274400,286328,298598,311213,324179,337500,351180,365226,379641,394431,409600,425152,441094,457429,474163,491300,508844,526802,545177,563975,583200,602856,622950,643485,664467,685900,707788,730138,752953,776239,800000, } };