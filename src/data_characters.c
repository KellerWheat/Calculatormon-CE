#include "data.h";

/* Item to evolution key
101 - thunder stone
102 - moon stone
103 - fire stone
104 - leaf stone
105 - water stone
106 - eevee
*/

/* name, element1, element2, health, attack, defence, special attack, special defence, speed, movelevels, moveids, evolvelevel, evolveid, evolvesfrom, xpdrop, xptype, catchrate */
struct pokemonType data_pokemon[] = {
	{" ",0,0,0,0,0,0,0,0,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,0,0,0,0},//0
	{"Bulbasaur",5,8,45,49,49,65,65,45,{0,0,7,13,20,27,34,41,48,0},{144,53,73,159,94,102,54,122,127,0},16,2,0,64,1,45},// 1
	{"Ivysaur",5,8,60,62,63,80,80,60,{13,22,30,38,46,54,0,0,0,0},{159,94,102,54,122,127,0,0,0,0},32,3,1,142,1,45},// 2
	{"Venusaur",5,8,80,82,83,100,100,80,{43,55,65,0,0,0,0,0,0,0},{54,122,127,0,0,0,0,0,0,0},0,0,2,236,1,45},// 3
	{"Charmander",2,0,39,52,43,60,50,65,{0,0,9,15,22,30,38,46,0,0},{112,53,40,74,101,121,46,44,0,0},16,5,0,62,1,45},// 4
	{"Charmeleon",2,0,58,64,58,80,65,80,{15,24,33,42,56,0,0,0,0,0},{74,101,121,46,44,0,0,0,0,0},36,6,4,142,1,45},// 5
	{"Charizard",2,10,78,84,78,109,85,100,{36,46,55,0,0,0,0,0,0,0},{121,46,44,0,0,0,0,0,0,0},0,0,5,240,1,45},// 6
	{"Squirtle",3,0,44,48,65,50,64,43,{0,0,8,15,22,28,35,42,0,0},{144,145,16,160,11,164,118,63,0,0},16,8,0,63,1,45},// 7
	{"Wartortle",3,0,59,63,80,65,80,58,{15,24,31,39,47,0,0,0,0,0},{160,11,164,118,63,0,0,0,0,0},36,9,7,142,1,45},// 8
	{"Blastoise",3,0,79,83,100,85,105,78,{24,31,42,52,0,0,0,0,0,0},{11,164,118,63,0,0,0,0,0,0},0,0,8,239,1,45},// 9
	{"Caterpie",12,0,45,30,35,20,20,45,{0,0,0,0,0,0,0,0,0,0},{144,134,0,0,0,0,0,0,0,0},7,11,0,39,2,255},// 10
	{"Metapod",12,0,50,20,55,25,25,30,{0,0,0,0,0,0,0,0,0,0},{57,0,0,0,0,0,0,0,0,0},10,12,10,72,2,120},// 11		
	{"Butterfree",12,10,60,45,50,90,80,70,{12,15,16,17,21,26,32,0,0,0},{21,94,136,122,140,162,97,0,0,0},0,0,11,178,2,45},// 12
	{"Weedle",12,8,40,35,30,20,20,50,{0,0,0,0,0,0,0,0,0,0},{95,134,0,0,0,0,0,0,0,0},7,14,0,39,2,255},// 13
	{"Kakuna",12,8,45,25,50,25,25,35,{0,0,0,0,0,0,0,0,0,0},{57,0,0,0,0,0,0,0,0,0},10,15,13,72,2,120},// 14
	{"Beedrill",12,8,65,90,40,45,80,75,{12,16,20,25,30,35,0,0,0,0},{50,49,157,101,92,4,0,0,0,0},0,0,14,178,2,45},// 15
	{"Pidgey",1,10,40,45,40,35,35,56,{0,5,12,19,28,36,44,0,0,0},{56,111,100,162,163,4,86,0,0,0},18,17,0,50,1,255},// 16
	{"Pidgeotto",1,10,63,60,55,50,50,71,{21,31,40,49,0,0,0,0,0,0},{162,163,4,86,0,0,0,0,0,0},36,18,16,122,1,120},// 17
	{"Pidgeot",1,10,83,80,75,70,70,101,{44,54,0,0,0,0,0,0,0,0},{4,86,0,0,0,0,0,0,0,0},0,0,17,216,1,45},// 18
	{"Rattata",1,0,30,56,35,25,35,72,{0,0,7,14,23,34,0,0,0,0},{144,145,100,65,49,139,0,0,0,0},20,20,0,51,2,255},// 19
	{"Raticate",1,0,55,81,60,50,70,97,{14,27,41,0,0,0,0,0,0,0},{65,49,139,0,0,0,0,0,0,0},0,0,19,145,2,127},// 20
	{"Spearow",1,10,40,60,30,31,31,70,{0,0,9,15,22,29,36,0,0,0},{90,53,74,50,86,37,4,0,0,0},20,22,0,52,2,255},// 21
	{"Fearow",1,10,65,90,65,61,61,100,{25,34,43,0,0,0,0,0,0,0},{86,37,4,0,0,0,0,0,0,0},0,0,21,155,2,90},// 22
	{"Ekans",8,0,35,60,44,40,54,55,{0,0,10,17,24,31,38,0,0,0},{165,74,95,11,52,113,2,0,0,0},22,24,0,58,2,255},// 23
	{"Arbok",8,0,60,95,69,65,79,80,{17,27,36,47,0,0,0,0,0,0},{11,52,113,2,0,0,0,0,0,0},0,0,23,157,2,90},// 24
	{"Pikachu",4,0,35,55,40,50,50,90,{0,0,9,16,26,33,43,0,0,0},{151,53,152,100,142,4,149,0,0,0},101,26,0,112,2,190},// 25
	{"Raichu",4,0,60,90,55,90,80,110,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,25,218,2,75},// 26
	{"Sandshrew",9,0,50,75,85,20,30,40,{0,10,17,24,31,38,0,0,0,0},{112,111,121,95,142,51,0,0,0,0},22,28,0,60,2,255},// 27
	{"Sandslash",9,0,75,100,110,45,55,65,{27,36,47,0,0,0,0,0,0,0},{95,142,51,0,0,0,0,0,0,0},0,0,37,158,2,90},// 28
	{"Nidoran",8,0,55,47,52,40,40,41,{0,0,8,14,21,29,36,43,0,0},{53,144,112,95,145,11,51,31,0,0},16,30,0,55,1,235},// 29
	{"Nidorina",8,0,70,62,67,55,55,56,{14,23,32,41,50,0,0,0,0,0},{95,145,11,51,31,0,0,0,0,0},102,31,29,128,1,120},// 30
	{"Nidoqueen",8,9,90,92,87,75,85,76,{14,23,0,0,0,0,0,0,0,0},{95,13,0,0,0,0,0,0,0,0},0,0,30,227,1,45},// 31
	{"Nidoran",8,0,46,57,40,40,40,50,{0,0,8,14,21,29,36,43,0,0},{74,144,61,95,49,50,62,31,0,0},16,33,0,55,1,235},// 32
	{"Nidorino",8,0,61,72,57,55,55,65,{23,32,41,50,0,0,0,0,0,0},{49,50,62,31,0,0,0,0,0,0},102,34,32,128,1,120},// 33
	{"Nidoking",8,9,81,102,77,85,75,85,{23,0,0,0,0,0,0,0,0,0},{148,0,0,0,0,0,0,0,0,0},0,0,33,227,1,45},// 34
	{"Clefairy",1,0,70,45,48,60,65,35,{0,0,13,18,24,31,39,48,0,0},{96,53,117,32,85,83,27,76,0,0},102,36,0,113,3,150},// 35
	{"Clefable",1,0,95,70,73,95,90,60,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,35,217,3,25},// 36
	{"Vulpix",2,0,38,41,40,50,65,65,{0,0,16,21,28,35,42,0,0,0},{40,145,100,107,20,46,44,0,0,0},103,38,0,60,2,190},// 37
	{"Ninetales",2,0,73,76,75,81,100,100,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,37,177,2,75},// 38
	{"Jigglypuff",1,0,115,45,20,45,25,20,{0,9,14,19,24,29,34,39,0,0},{117,96,29,27,32,106,13,34,0,0},102,40,0,95,3,170},// 39
	{"Wigglytuff",1,0,140,70,45,85,50,45,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,39,196,3,50},// 40
	{"Zubat",8,10,40,45,35,30,40,55,{0,10,15,21,28,36,0,0,0,0},{72,140,11,20,163,58,0,0,0,0},22,42,0,49,2,255},// 41
	{"Golbat",8,10,75,80,70,65,75,90,{32,43,0,0,0,0,0,0,0,0},{163,58,0,0,0,0,0,0,0,0},0,0,41,159,2,90},// 42
	{"Oddish",5,8,45,50,55,75,65,30,{0,15,17,19,24,33,46,0,0,0},{1,94,136,122,2,91,127,0,0,0},21,44,0,64,1,255},// 43
	{"Gloom",5,8,60,65,70,85,75,40,{28,38,52,0,0,0,0,0,0,0},{2,91,127,0,0,0,0,0,0,0},104,45,43,138,1,120},// 44
	{"Vileplume",5,8,75,80,85,110,90,50,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,44,221,1,45},// 45
	{"Paras",12,5,35,70,55,45,55,25,{0,13,20,27,34,41,0,0,0,0},{112,136,72,131,121,54,0,0,0,0},24,47,0,57,2,190},// 46
	{"Parasect",12,5,60,95,80,60,80,30,{30,39,48,0,0,0,0,0,0,0},{131,121,54,0,0,0,0,0,0,0},0,0,46,142,2,75},// 47
	{"Venonat",12,8,60,55,50,40,55,45,{0,0,24,27,30,35,38,43,0,0},{144,29,94,72,136,97,122,98,0,0},31,49,0,61,2,190},// 48
	{"Venomoth",12,8,70,65,60,90,75,90,{38,43,50,0,0,0,0,0,0,0},{97,122,98,0,0,0,0,0,0,0},0,0,48,158,2,75},// 49
	{"Diglett",9,0,10,55,25,35,45,95,{0,15,19,24,31,40,0,0,0,0},{112,53,28,111,121,38,0,0,0,0},26,51,0,53,2,255},// 50
	{"Dugtrio",9,0,35,100,50,50,70,120,{35,47,0,0,0,0,0,0,0,0},{121,38,0,0,0,0,0,0,0,0},0,0,50,149,2,50},// 51
	{"Meowth",1,0,40,45,35,40,40,90,{0,0,12,17,24,33,44,0,0,0},{112,53,11,89,113,51,121,0,0,0},28,53,0,58,2,255},// 52
	{"Persian",1,0,65,70,60,65,65,115,{37,51,0,0,0,0,0,0,0,0},{51,121,0,0,0,0,0,0,0,0},0,0,52,154,2,90},// 53
	{"Psyduck",3,0,50,52,48,65,50,55,{0,28,31,36,43,52,0,0,0,0},{112,145,29,21,51,63,0,0,0,0},33,55,0,64,2,190},// 54
	{"Golduck",3,0,80,82,78,95,80,85,{39,48,59,0,0,0,0,0,0,0},{21,51,63,0,0,0,0,0,0,0},0,0,54,175,2,75},// 55
	{"Mankey",7,0,40,80,35,35,45,70,{0,0,15,21,27,33,39,0,0,0},{112,74,70,50,49,114,148,0,0,0},28,57,0,61,2,190},// 56
	{"Primeape",7,0,65,105,60,60,70,95,{37,46,0,0,0,0,0,0,0,0},{114,148,0,0,0,0,0,0,0,0},0,0,56,159,2,75},// 57
	{"Growlithe",2,0,55,70,45,70,50,60,{0,0,18,23,30,39,50,0,0,0},{11,107,40,74,146,4,46,0,0,0},103,59,0,70,0,190},// 58
	{"Arcanine",2,0,90,110,80,100,80,95,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,58,194,0,75},// 59
	{"Poliwag",3,0,40,50,40,40,40,90,{0,16,19,25,31,38,45,0,0,0},{16,66,160,32,13,5,63,0,0,0},25,61,0,60,1,255},// 60
	{"Poliwhirl",3,0,65,65,65,50,50,90,{26,33,41,49,0,0,0,0,0,0},{32,13,5,63,0,0,0,0,0,0},105,62,60,135,1,120},// 61
	{"Poliwrath",3,7,90,95,95,70,90,70,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,61,230,1,45},// 62
	{"Abra",11,0,25,20,15,105,55,90,{0,0,0,0,0,0,0,0,0,0},{147,0,0,0,0,0,0,0,0,0},16,64,0,62,1,200},// 63
	{"Kadabra",11,0,40,35,30,120,70,105,{0,0,20,27,31,38,42,0,0,0},{147,21,29,97,104,98,105,0,0,0},36,65,63,140,1,100},// 64
	{"Alakazam",11,0,55,50,45,135,95,120,{20,27,31,38,42,0,0,0,0,0},{29,97,104,98,105,0,0,0,0,0},0,0,64,225,1,50},// 65
	{"Machop",7,0,70,80,50,35,35,35,{0,20,25,32,39,46,0,0,0,0},{70,78,74,49,114,137,0,0,0,0},28,67,0,61,1,180},// 66
	{"Machoke",7,0,80,100,70,50,60,45,{36,44,52,0,0,0,0,0,0,0},{49,114,137,0,0,0,0,0,0,0},40,68,66,142,1,90},// 67
	{"Machamp",7,0,90,130,80,65,85,55,{36,44,52,0,0,0,0,0,0,0},{49,114,137,0,0,0,0,0,0,0},0,0,67,227,1,45},// 68
	{"Bellsprout",5,8,50,75,35,70,30,40,{0,0,13,15,18,21,26,33,42,0},{159,54,165,94,122,136,2,102,120,0},21,70,0,60,1,255},// 69
	{"Weepinbell",5,8,65,90,50,85,45,55,{23,29,38,49,0,0,0,0,0,0},{136,2,102,120,0,0,0,0,0,0},104,71,69,137,1,120},// 70
	{"Victreebel",5,8,80,105,65,100,70,70,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,70,221,1,45},// 71
	{"Tentacool",3,8,40,40,35,50,100,70,{0,7,13,18,22,27,33,40,48,0},{2,140,165,95,160,22,8,113,63,0},30,73,0,67,0,190},// 72
	{"Tentacruel",3,8,80,70,65,80,120,100,{35,43,50,0,0,0,0,0,0,0},{8,113,63,0,0,0,0,0,0,0},0,0,72,180,0,60},// 73
	{"Geodude",13,9,40,80,100,30,30,20,{0,11,16,21,26,31,36,0,0,0},{144,27,109,115,57,38,41,0,0,0},25,75,0,60,1,255},// 74
	{"Graveler",13,9,55,95,115,45,45,35,{29,36,43,0,0,0,0,0,0,0},{57,38,41,0,0,0,0,0,0,0},40,76,74,137,1,120},// 75
	{"Golem",13,9,80,120,130,55,65,45,{29,36,43,0,0,0,0,0,0,0},{57,38,41,0,0,0,0,0,0,0},0,0,75,223,1,45},// 76
	{"Ponyta",2,0,50,85,55,65,65,90,{0,30,32,35,39,43,48,0,0,0},{40,145,132,53,44,146,4,0,0,0},40,78,0,82,2,190},// 77
	{"Rapidash",2,0,65,100,70,80,80,105,{47,55,0,0,0,0,0,0,0,0},{146,4,0,0,0,0,0,0,0,0},0,0,77,175,2,60},// 78
	{"Slowpoke",3,11,90,65,65,40,40,15,{0,18,22,27,33,40,48,0,0,0},{21,29,59,53,160,5,98,0,0,0},37,80,0,63,2,190},// 79
	{"Slowbro",3,11,95,75,110,100,80,30,{44,55,0,0,0,0,0,0,0,0},{5,98,0,0,0,0,0,0,0,0},0,0,79,172,2,75},// 80
	{"Magnemite",4,17,25,35,70,95,55,45,{0,21,25,29,35,41,47,0,0,0},{144,128,151,140,152,142,113,0,0,0},30,82,0,65,2,190},// 81
	{"Magneton",4,17,50,60,95,120,70,70,{38,46,54,0,0,0,0,0,0,0},{152,142,113,0,0,0,0,0,0,0},0,0,81,163,2,60},// 82
	{"Farfetch'd",1,10,52,90,55,58,62,60,{0,0,7,15,23,31,39,0,0,0},{90,111,74,50,143,4,121,0,0,0},0,0,0,132,2,45},// 83
	{"Doduo",1,10,35,85,45,35,35,75,{0,20,24,30,36,40,44,0,0,0},{90,53,50,37,101,156,4,0,0,0},31,85,0,62,2,190},// 84
	{"Dodrio",1,10,60,110,70,60,60,110,{39,45,51,0,0,0,0,0,0,0},{101,156,4,0,0,0,0,0,0,0},0,0,84,165,2,45},// 85
	{"Seel",3,0,65,45,55,45,70,45,{0,30,35,40,45,50,0,0,0,0},{59,53,6,106,146,67,0,0,0,0},34,87,0,65,2,190},// 86
	{"Dewgong",3,6,90,70,80,70,95,70,{35,44,50,56,0,0,0,0,0,0},{6,106,146,67,0,0,0,0,0,0},0,0,86,166,2,75},// 87
	{"Grimer",8,0,80,80,50,40,50,25,{0,0,30,33,37,42,48,55,0,0},{96,29,93,85,123,57,113,3,0,0},38,89,0,65,2,190},// 88
	{"Muk",8,0,105,105,75,65,100,50,{45,53,60,0,0,0,0,0,0,0},{57,113,3,0,0,0,0,0,0,0},0,0,88,175,2,75},// 89
	{"Shellder",3,0,30,65,100,45,25,40,{0,0,18,23,30,39,50,0,0,0},{144,164,140,18,6,74,67,0,0,0},105,91,0,61,0,190},// 90
	{"Cloyster",3,6,50,95,180,85,45,70,{50,0,0,0,0,0,0,0,0,0},{129,0,0,0,0,0,0,0,0,0},0,0,90,184,0,60},// 91
	{"Gastly",14,8,30,35,30,100,35,80,{0,0,0,27,35,0,0,0,0,0},{75,20,88,66,36,0,0,0,0,0},25,93,0,62,1,190},// 92
	{"Haunter",14,8,45,50,45,115,55,95,{29,38,0,0,0,0,0,0,0,0},{66,36,0,0,0,0,0,0,0,0},40,94,92,142,1,90},// 93
	{"Gengar",14,8,60,65,60,130,75,110,{29,38,0,0,0,0,0,0,0,0},{66,36,0,0,0,0,0,0,0,0},0,0,93,225,1,45},// 94
	{"Onix",13,9,35,45,160,30,45,70,{0,0,15,19,25,33,43,0,0,0},{144,113,10,109,101,120,57,0,0,0},0,0,0,77,2,45},// 95
	{"Drowzee",11,0,60,48,45,43,90,42,{0,0,12,17,24,29,32,37,0,0},{96,66,29,21,59,93,98,79,0,0},26,97,0,66,2,190},// 96
	{"Hypno",11,0,85,73,70,73,115,67,{33,37,43,0,0,0,0,0,0,0},{93,98,79,0,0,0,0,0,0,0},0,0,96,169,2,75},// 97
	{"Krabby",3,0,30,105,90,25,25,50,{0,0,20,25,30,35,40,0,0,0},{16,74,158,55,132,25,57,0,0,0},28,99,0,65,2,225},// 98
	{"Kingler",3,0,55,130,115,50,50,75,{34,42,49,0,0,0,0,0,0,0},{132,25,57,0,0,0,0,0,0,0},0,0,98,166,2,60},// 99
	{"Voltorb",4,0,40,30,50,55,55,100,{0,0,17,22,29,36,43,0,0,0},{144,113,128,115,76,142,41,0,0,0},30,101,0,66,2,190},// 100
	{"Electrode",4,0,60,50,70,80,80,150,{40,50,0,0,0,0,0,0,0,0},{142,41,0,0,0,0,0,0,0,0},0,0,100,172,2,60},// 101
	{"Exeggcute",5,11,60,40,80,60,45,40,{0,0,25,28,32,37,42,48,0,0},{7,66,105,73,136,94,127,122,0,0},104,103,0,65,0,90},// 102
	{"Exeggutor",5,11,95,95,85,125,75,55,{28,0,0,0,0,0,0,0,0,0},{132,0,0,0,0,0,0,0,0,0},0,0,102,186,0,45},// 103
	{"Cubone",9,0,50,50,95,40,50,35,{0,0,25,31,38,43,46,0,0,0},{14,53,74,49,148,15,101,0,0,0},28,105,0,64,2,190},// 104
	{"Marowak",9,0,60,80,110,50,80,45,{33,41,48,55,0,0,0,0,0,0},{49,148,15,101,0,0,0,0,0,0},0,0,104,149,2,75},// 105
	{"Hitmonlee",7,0,50,120,53,35,110,87,{0,0,33,38,43,48,53,0,0,0},{31,79,110,69,49,60,81,0,0,0},0,0,0,159,2,45},// 106
	{"Hitmonchan",7,0,50,105,79,35,110,76,{0,0,33,38,43,48,53,0,0,0},{19,4,43,68,150,82,24,0,0,0},0,0,0,159,2,45},// 107
	{"Lickitung",1,0,90,55,75,60,75,30,{0,0,7,15,23,31,39,0,0,0},{165,140,132,29,27,120,113,0,0,0},0,0,0,77,2,45},// 108
	{"Koffing",8,0,40,65,95,60,45,35,{0,0,32,37,40,45,48,0,0,0},{144,124,123,125,115,58,41,0,0,0},35,110,0,68,2,190},// 109
	{"Weezing",8,0,65,90,120,85,70,60,{39,43,49,53,0,0,0,0,0,0},{125,115,58,41,0,0,0,0,0,0},0,0,109,172,2,60},// 110
	{"Rhyhorn",9,13,80,85,95,30,30,25,{0,30,35,40,45,50,55,0,0,0},{61,132,145,50,62,74,146,0,0,0},42,112,0,69,0,120},// 111
	{"Rhydon",9,13,105,130,120,45,45,40,{48,55,64,0,0,0,0,0,0,0},{62,74,146,0,0,0,0,0,0,0},0,0,111,170,0,60},// 112
	{"Chansey",1,0,250,5,5,35,105,50,{0,0,24,30,38,44,48,54,0,0},{96,32,117,53,85,27,76,34,0,0},0,0,0,395,3,30},// 113
	{"Tangela",5,0,65,55,115,100,40,60,{0,0,29,32,36,39,45,49,0,0},{22,1,10,94,136,122,120,54,0,0},0,0,0,87,2,45},// 114
	{ "Kangaskhan",1,0,105,95,80,40,80,90,{0,0,26,31,36,41,46,0,0,0},{19,101,11,145,82,74,30,0,0,0},0,0,0,172,2,45},// 115
	{ "Horsea",3,0,30,40,70,70,25,60,{0,19,24,30,37,45,0,0,0,0},{16,125,74,160,4,63,0,0,0,0},32,117,0,59,2,225},// 116
	{ "Seadra",3,0,55,65,95,95,45,85,{41,52,0,0,0,0,0,0,0,0},{4,63,0,0,0,0,0,0,0,0},0,0,116,154,2,75},// 117
	{ "Goldeen",3,0,45,67,60,35,50,63,{0,0,19,24,30,37,45,54,0,0},{90,145,140,61,50,161,62,4,0,0},33,119,0,64,2,225},// 118
	{ "Seaking",3,0,80,92,65,65,80,68,{39,48,54,0,0,0,0,0,0,0},{161,62,4,0,0,0,0,0,0,0},0,0,118,158,2,60},// 119
	{ "Staryu",3,0,30,45,55,70,55,85,{0,17,22,27,32,37,42,47,0,0},{144,160,57,104,142,85,76,63,0,0},105,121,0,68,0,225},// 120
	{ "Starmie",3,11,60,75,85,100,85,115,{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},0,0,120,182,0,60 },// 121
	{ "Mr.Mime",11,0,40,45,65,100,120,90,{0,0,23,31,39,47,0,0,0,0},{21,8,76,32,79,138,0,0,0,0},0,0,0,161,2,45 },// 122
	{ "Scyther",12,10,70,110,80,55,80,105,{0,17,20,24,29,35,42,0,0,0},{100,74,49,33,121,143,4,0,0,0},0,0,0,100,2,45 },// 123
	{ "Jynx",6,11,65,50,35,115,95,95,{0,0,15,23,31,39,47,0,0,0},{96,77,117,32,68,79,12,0,0,0},0,0,0,159,2,45 },// 124
	{ "Electabuzz",4,0,65,83,57,95,85,105,{0,0,34,37,42,49,54,0,0,0},{100,74,151,113,150,76,149,0,0,0},0,0,0,172,2,45 },// 125
	{ "Magmar",2,0,65,95,57,100,85,93,{0,36,39,43,48,52,55,0,0,0},{40,74,20,43,125,124,46,0,0,0},0,0,0,173,2,45 },// 126
	{ "Pinsir",12,0,65,125,100,55,70,85,{0,25,30,36,43,49,54,0,0,0},{158,114,55,49,57,121,143,0,0,0},0,0,0,175,0,45 },// 127
	{ "Tauros",1,0,75,100,95,40,70,110,{0,21,28,35,44,51,0,0,0,0},{144,132,145,74,101,146,0,0,0,0},0,0,0,172,0,45 },// 128
	{ "Magikarp",3,0,20,10,55,15,20,80,{0,15,0,0,0,0,0,0,0,0},{130,144,0,0,0,0,0,0,0,0},20,130,0,40,0,255 },// 129
	{ "Gyarados",3,10,95,125,79,60,100,81,{20,25,32,41,52,0,0,0,0,0},{11,35,74,63,64,0,0,0,0,0},0,0,129,189,0,45 },// 130
	{ "Lapras",3,6,130,85,80,85,95,60,{0,0,16,20,25,31,38,46,0,0},{160,53,117,87,13,20,67,63,0,0},0,0,0,187,0,45 },// 131
	{ "Ditto",1,0,48,48,48,48,48,48,{0,0,0,0,0,0,0,0,0,0},{155,0,0,0,0,0,0,0,0,0},0,0,0,101,2,35 },// 132
	{ "Eevee",1,0,55,55,50,45,65,55,{0,0,27,31,37,45,0,0,0,0},{144,111,100,145,11,146,0,0,0,0},106,0,0,65,2,45 },// 133
	{ "Vaporeon",3,0,130,65,60,110,95,65,{27,31,37,40,42,44,48,54,0,0},{100,160,145,11,3,58,87,63,0,0},0,0,133,184,2,45 },// 134
	{ "Jolteon",4,0,65,65,60,110,95,130,{27,31,37,40,42,44,48,54,0,0},{100,151,145,152,31,4,92,149,0,0},0,0,133,184,2,45 },// 135
	{ "Flareon",2,0,65,130,60,95,110,65,{27,31,37,40,42,44,48,54,0,0},{100,40,145,11,74,44,101,46,0,0},0,0,133,184,2,45 },// 136
	{ "Porygon",1,0,65,60,70,85,75,40,{0,0,0,23,28,35,42,0,0,0},{144,116,23,97,104,4,156,0,0,0},0,0,0,79,2,45 },// 137
	{ "Omanyte",13,3,35,40,100,90,55,35,{0,0,34,39,46,53,0,0,0,0},{160,164,61,74,129,63,0,0,0,0},40,139,0,71,2,45 },// 138
	{ "Omastar",13,3,70,60,125,115,70,55,{44,49,0,0,0,0,0,0,0,0},{129,63,0,0,0,0,0,0,0,0},0,0,138,173,2,45 },// 139
	{ "Kabuto",13,3,30,80,90,55,45,55,{0,0,34,39,44,49,0,0,0,0},{112,57,1,121,74,63,0,0,0,0},40,141,0,71,2,45 },// 140
	{ "Kabutops",13,3,60,115,105,65,70,80,{46,53,0,0,0,0,0,0,0,0},{74,63,0,0,0,0,0,0,0,0},0,0,140,173,2,45 },// 141
	{ "Aerodactyl",13,10,80,105,65,60,75,130,{0,0,33,38,45,54,0,0,0,0},{163,4,140,11,146,64,0,0,0,0},0,0,0,180,0,45 },// 142
	{ "Snorlax",1,0,160,110,65,65,110,30,{0,0,0,35,41,48,56,0,0,0},{59,5,106,13,57,34,64,0,0,0},0,0,0,189,0,25 },// 143
	{ "Articuno",6,10,90,85,100,95,125,85,{0,0,51,55,60,0,0,0,0,0},{90,67,12,4,87,0,0,0,0,0},0,0,0,261,0,3 },// 144
	{ "Zapdos",4,10,90,90,85,125,90,100,{0,0,51,55,60,0,0,0,0,0},{151,37,149,4,76,0,0,0,0,0},0,0,0,261,0,3 },// 145
	{ "Moltres",2,10,90,100,90,125,85,90,{0,0,51,55,60,0,0,0,0,0},{90,44,74,4,119,0,0,0,0,0},0,0,0,261,0,3 },// 146
	{ "Dratini",15,0,41,64,45,50,50,50,{0,0,10,20,30,40,50,0,0,0},{165,74,152,4,120,35,64,0,0,0},30,148,0,60,0,45 },// 147
	{ "Dragonair",15,0,61,84,65,70,70,70,{35,45,55,0,0,0,0,0,0,0},{120,35,64,0,0,0,0,0,0,0},55,147,147,147,0,27 },// 148
	{ "Dragonite",15,10,91,134,95,100,100,80,{60,0,0,0,0,0,0,0,0,0},{64,0,0,0,0,0,0,0,0,0},0,0,148,270,0,9 },// 149
	{ "Mewtwo",11,0,106,110,90,154,90,130,{0,0,0,63,66,70,75,81,0,0},{21,29,142,8,98,104,87,5,0,0},0,0,0,306,0,3 },// 150
	{ "Mew",11,0,100,100,100,100,100,100,{0,10,20,30,40,0,0,0,0,0},{96,155,82,83,98,0,0,0,0,0},0,0,0,270,1,45 },// 151
	{ "Ricardo",2,11,255,255,255,255,255,255,{0,0,0,0,0,0,0,0,0,0},{166,0,0,0,0,0,0,0,0,0},0,0,0,0,3,0 },// 152
};