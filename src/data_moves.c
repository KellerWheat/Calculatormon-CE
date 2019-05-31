#include "data.h";

/* movetypes:
0 - physical
1 - special
2 - raise user stat (power is number of times)
3 - lower non-user stat (power is number of times)
4 - status move (power is meaningless, but a normal move can also have a status effect)
5 - specific damage
6 - leave battle
7 - switch opponent/leave battle (switch opponent not working)
8 - recover hp
9 - damage is user level
10 - damage is 50-150% of user level
11 - damage is half of enemy hp
12 - haze
13 - rest
14 - disable
15 - one hit ko
16 - random move
17 - copy enemy move
18 - transform  (NOT WORKING)
19 - counter
*/


/* status types:
0 - attack
1 - defence
2 - special attack
3 - special defence
4 - speed
6 - evasiveness
5 - accuracy

7 - poison
8 - burn
9 - paralysis
10 - sleep
11 - frozen

12 - confused
13 - leech seed
14 - trap
15 - reflect
16 - light screen
17 - mist
18 - flinch

19-26 and 31-34 do not need statuschance to work:

19 - recoil
20 - user faints
21 - drain life
22 - hit 2-5 times
23 - hit 2 times
24 - high critical hit ratio
25 - jump kick
26 - dream eater

27 - increase critical hit ratio
28 - raise attack and defence
29 - raise attack when hit
30 - paralyse/burn/freeze

31 - attack first (NOT WORKING)
32 - attack 2-3 turns
33 - charge attack
34 - fly/dig
*/

/* name, element, type, power, accuracy, pp, statustype, statuschance */
struct move data_moves[] = {
	{"Empty",0,0,0,0,0,0,0},// 0
	{"Absorb",5,1,20,100,25,21,0},// 1 - User recovers half the HP inflicted on opponent.
	{"Acid",8,1,40,100,30,3,10},// 2 - May lower opponent's Special Defense.
	{"Acid Armor",8,2,2,100,20,1,0},// 3 - Sharply raises user's Defense.
	{"Agility",11,2,2,100,30,4,0},// 4 - Sharply raises user's Speed.
	{"Amnesia",11,2,2,100,20,3,0},// 5 - Sharply raises user's Special Defense.
	{"Aurora Beam",6,1,65,100,20,0,10},// 6 - May lower opponent's Attack.
	{"Barrage",1,0,15,85,20,22,0},// 7 - Hits 2-5 times in one turn.
	{"Barrier",11,2,2,100,20,1,0},// 8 - Sharply raises user's Defense.
	{"Bide",1,0,0,100,10,0,0},// 9 - User takes damage for two turns then strikes back double. (NOT WORKING)
	{"Bind",1,0,15,85,20,14,100},// 10 - Traps opponent, damaging them for 4-5 turns.
	{"Bite",16,0,60,100,25,18,10},// 11 - May cause flinching.
	{"Blizzard",6,1,110,70,5,11,10},// 12 - May freeze opponent.
	{"Body Slam",1,0,85,100,15,9,10},// 13 - May paralyze opponent.
	{"Bone Club",9,0,65,85,20,18,10},// 14 - May cause flinching.
	{"Bonemerang",9,0,50,90,10,23,0},// 15 - Hits twice in one turn.
	{"Bubble",3,1,40,100,30,4,10},// 16 - May lower opponent's Speed.
	{"Bubble Beam",3,1,65,100,20,4,10},// 17 - May lower opponent's Speed.
	{"Clamp",3,0,35,85,10,14,100},// 18 - Traps opponent, damaging them for 4-5 turns.
	{"Comet Punch",1,0,18,85,15,22,0},// 19 - Hits 2-5 times in one turn.
	{"Confuse Ray",14,4,0,100,10,12,0},// 20 - Confuses opponent.
	{"Confusion",11,1,50,100,25,12,10},// 21 - May confuse opponent.
	{"Constrict",1,0,10,100,35,4,10},// 22 - May lower opponent's Speed by one stage.
	{"Conversion",1,0,0,100,30,0,0},// 23 - Changes user's type to that of its first move.  (NOT WORKING)
	{"Counter",7,19,0,100,20,0,0},// 24 - When hit by a Physical Attack, user strikes back with 2x power.
	{"Crabhammer",3,0,100,90,10,24,0},// 25 - High critical hit ratio.
	{"Cut",1,0,50,95,30,0,0},// 26 -  
	{"Defense Curl",1,2,1,100,40,1,0},// 27 - Raises user's Defense.
	{"Dig",9,0,80,100,10,34,0},// 28 - Digs underground on first turn, attacks on second. Can also escape from caves.
	{"Disable",1,14,0,100,20,0,0},// 29 - Opponent can't use its last attack for a few turns.
	{"Dizzy Punch",1,0,70,100,10,12,10},// 30 - May confuse opponent.
	{"Double Kick",7,0,30,100,30,23,0},// 31 - Hits twice in one turn.
	{"Double Slap",1,0,15,85,10,22,0},// 32 - Hits 2-5 times in one turn.
	{"Double Team",1,2,1,100,15,6,0},// 33 - Raises user's Evasiveness.
	{"Double-Edge",1,0,120,100,15,19,0},// 34 - User receives recoil damage.
	{"Dragon Rage",15,5,40,100,10,0,0},// 35 - Always inflicts 40 HP.
	{"Dream Eater",11,1,100,100,15,26,0},// 36 - User recovers half the HP inflicted on a sleeping opponent.
	{"Drill Peck",10,0,80,100,20,0,0},// 37 -  
	{"Earthquake",9,0,100,100,10,0,0},// 38 - Power is doubled if opponent is underground from using Dig.
	{"Egg Bomb",1,0,100,75,10,0,0},// 39 -  
	{"Ember",2,1,40,100,25,8,0},// 40 - May burn opponent.
	{"Explosion",1,0,250,100,5,20,0},// 41 - User faints.
	{"Fire Blast",2,1,110,85,5,8,10},// 42 - May burn opponent.
	{"Fire Punch",2,0,75,100,15,8,10},// 43 - May burn opponent.
	{"Fire Spin",2,1,35,85,15,14,100},// 44 - Traps opponent, damaging them for 4-5 turns.
	{"Fissure",9,15,0,100,5,0,0},// 45 - One-Hit-KO, if it hits.
	{"Flamethrower",2,1,90,100,15,8,10},// 46 - May burn opponent.
	{"Flash",1,3,1,100,20,5,0},// 47 - Lowers opponent's Accuracy.
	{"Fly",10,0,90,95,15,34,0},// 48 - Flies up on first turn, attacks on second turn.
	{"Focus Energy",1,4,0,100,30,27,0},// 49 - Increases critical hit ratio.
	{"Fury Attack",1,0,15,85,20,22,0},// 50 - Hits 2-5 times in one turn.
	{"Fury Swipes",1,0,18,80,15,22,0},// 51 - Hits 2-5 times in one turn.
	{"Glare",1,4,0,100,30,9,0},// 52 - Paralyzes opponent.
	{"Growl",1,3,1,100,40,0,0},// 53 - Lowers opponent's Attack.
	{"Growth",1,4,1,100,40,28,0},// 54 - Raises user's Attack and Special Attack.
	{"Guillotine",1,15,0,100,5,0,0},// 55 - One-Hit-KO, if it hits.
	{"Gust",10,1,40,100,35,0,0},// 56 - Hits Pokémon using Fly/Bounce with double power.
	{"Harden",1,2,1,100,30,1,0},// 57 - Raises user's Defense.
	{"Haze",6,12,0,100,30,0,0},// 58 - Resets all stat changes.
	{"Headbutt",1,0,70,100,15,18,10},// 59 - May cause flinching.
	{"High Jump Kick",7,0,130,90,10,25,0},// 60 - If it misses, the user loses half their HP.
	{"Horn Attack",1,0,65,100,25,0,0},// 61 -  
	{"Horn Drill",1,15,0,100,5,0,0},// 62 - One-Hit-KO, if it hits.
	{"Hydro Pump",3,1,110,80,5,0,0},// 63 -  
	{"Hyper Beam",1,1,150,90,5,33,0},// 64 - User must recharge next turn.
	{"Hyper Fang",1,0,80,90,15,18,10},// 65 - May cause flinching.
	{"Hypnosis",11,4,0,60,20,10,0},// 66 - Puts opponent to sleep.
	{"Ice Beam",6,1,90,100,10,11,10},// 67 - May freeze opponent.
	{"Ice Punch",6,0,75,100,15,11,10},// 68 - May freeze opponent.
	{"Jump Kick",7,0,100,95,10,25,0},// 69 - If it misses, the user loses half their HP.
	{"Karate Chop",7,0,50,100,25,24,0},// 70 - High critical hit ratio.
	{"Kinesis",11,3,1,80,15,5,0},// 71 - Lowers opponent's Accuracy.
	{"Leech Life",12,0,80,100,10,21,0},// 72 - User recovers half the HP inflicted on opponent.
	{"Leech Seed",5,4,0,90,10,13,0},// 73 - Drains HP from opponent each turn.
	{"Leer",1,3,1,100,30,1,0},// 74 - Lowers opponent's Defense.
	{"Lick",14,0,30,100,30,9,19},// 75 - May paralyze opponent.
	{"Light Screen",11,4,0,100,30,16,0},// 76 - Halves damage from Special attacks for 5 turns.
	{"Lovely Kiss",1,4,0,75,10,10,0},// 77 - Puts opponent to sleep.
	{"Low Kick",7,0,45,100,20,0,0},// 78 - The heavier the opponent, the stronger the attack.  (CHANGED to fixed damage)
	{"Meditate",11,2,1,100,40,0,0},// 79 - Raises user's Attack.
	{"Mega Drain",5,1,40,100,15,21,0},// 80 - User recovers half the HP inflicted on opponent.
	{"Mega Kick",1,0,120,75,5,0,0},// 81 -  
	{"Mega Punch",1,0,80,85,20,0,0},// 82 -  
	{"Metronome",1,16,0,100,10,0,0},// 83 - User performs almost any move in the game at random.
	{"Mimic",1,17,0,100,10,0,0},// 84 - Copies the opponent's last move.
	{"Minimize",1,2,2,100,10,6,0},// 85 - Sharply raises user's Evasiveness.
	{"Mirror Move",10,17,0,100,20,0,0},// 86 - User performs the opponent's last move.
	{"Mist",6,4,0,100,30,17,0},// 87 - User's stats cannot be changed for a period of time.
	{"Night Shade",14,9,0,100,15,0,0},// 88 - Inflicts damage equal to user's level.
	{"Pay Day",1,0,40,100,20,0,0},// 89 - A small amount of money is gained after the battle resolves.  (NO MONEY)
	{"Peck",10,0,35,100,35,0,0},// 90 -  
	{"Petal Dance",5,1,120,100,10,32,0},// 91 - User attacks for 2-3 turns but then becomes confused.
	{"Pin Missile",12,0,25,95,20,22,0},// 92 - Hits 2-5 times in one turn.
	{"Poison Gas",8,4,0,90,40,7,0},// 93 - Poisons opponent.
	{"Poison Powder",8,4,0,75,35,7,0},// 94 - Poisons opponent.
	{"Poison Sting",8,0,15,100,35,7,20},// 95 - May poison the opponent.
	{"Pound",1,0,40,100,35,0,0},// 96 -  
	{"Psybeam",11,1,65,100,20,12,0},// 97 - May confuse opponent.
	{"Psychic",11,1,90,100,10,3,10},// 98 - May lower opponent's Special Defense.
	{"Psywave",11,10,0,80,15,0,0},// 99 - Inflicts damage 50-150% of user's level.
	{"Quick Attack",1,0,40,100,30,31,0},// 100 - User attacks first.
	{"Rage",1,0,20,100,20,29,0},// 101 - Raises user's Attack when hit.
	{"Razor Leaf",5,0,55,95,25,24,0},// 102 - High critical hit ratio.
	{"Razor Wind",1,1,80,100,10,33,0},// 103 - Charges on first turn, attacks on second. High critical hit ratio.  (NO HIGH CRIT RATIO)
	{"Recover",1,8,0,100,10,0,0},// 104 - User recovers half its max HP.
	{"Reflect",11,4,0,100,20,15,0},// 105 - Halves damage from Physical attacks for 5 turns.
	{"Rest",11,13,0,100,10,0,0},// 106 - User sleeps for 2 turns, but user is fully healed.
	{"Roar",1,7,0,100,20,0,0},// 107 - In battles, the opponent switches. In the wild, the Pokémon runs.
	{"Rock Slide",13,0,75,90,10,18,0},// 108 - May cause flinching.
	{"Rock Throw",13,0,50,90,15,0,0},// 109 -  
	{"Rolling Kick",7,0,60,85,15,18,0},// 110 - May cause flinching.
	{"Sand Attack",9,3,1,100,15,5,0},// 111 - Lowers opponent's Accuracy.
	{"Scratch",1,0,40,100,35,0,0},// 112 -  
	{"Screech",1,3,2,85,40,1,0},// 113 - Sharply lowers opponent's Defense.
	{"Seismic Toss",7,9,0,100,20,0,0},// 114 - Inflicts damage equal to user's level.
	{"Self-Destruct",1,0,200,100,5,20,0},// 115 - User faints.
	{"Sharpen",1,2,1,100,30,0,0},// 116 - Raises user's Attack.
	{"Sing",1,4,0,55,15,10,0},// 117 - Puts opponent to sleep.
	{"Skull Bash",1,0,130,100,10,33,0},// 118 - Raises Defense on first turn, attacks on second. (NO DEFENCE)
	{"Sky Attack",10,0,140,90,5,33,0},// 119 - Charges on first turn, attacks on second. May cause flinching.  (NO FLINCH)
	{"Slam",1,0,80,75,20,0,0},// 120 -  
	{"Slash",1,0,70,100,20,24,0},// 121 - High critical hit ratio.
	{"Sleep Powder",5,4,0,75,15,10,0},// 122 - Puts opponent to sleep.
	{"Sludge",8,1,65,100,20,7,10},// 123 - May poison opponent.
	{"Smog",8,1,30,70,20,7,10},// 124 - May poison opponent.
	{"Smokescreen",1,3,1,100,20,5,0},// 125 - Lowers opponent's Accuracy.
	{"Soft-Boiled",1,8,0,100,10,0,0},// 126 - User recovers half its max HP.
	{"Solar Beam",5,1,120,100,10,33,0},// 127 - Charges on first turn, attacks on second.
	{"Sonic Boom",1,5,20,90,20,0,0},// 128 - Always inflicts 20 HP.
	{"Spike Cannon",1,0,20,100,15,22,0},// 129 - Hits 2-5 times in one turn.
	{"Splash",1,0,0,100,40,0,0},// 130 - Doesn't do ANYTHING.
	{"Spore",5,4,0,100,15,10,0},// 131 - Puts opponent to sleep.
	{"Stomp",1,0,65,100,20,18,10},// 132 - May cause flinching.
	{"Strength",1,0,80,100,15,0,0},// 133 -  
	{"String Shot",12,3,2,95,40,4,0},// 134 - Sharply lowers opponent's Speed.
	{"Struggle",1,0,50,100,0,19,0},// 135 - Only usable when all PP are gone. Hurts the user.
	{"Stun Spore",5,4,0,75,30,9,0},// 136 - Paralyzes opponent.
	{"Submission",7,0,80,80,20,19,0},// 137 - User receives recoil damage.
	{"Substitute",1,0,0,100,10,0,0},// 138 - Uses HP to creates a decoy that takes hits.  (NOT WORKING)
	{"Super Fang",1,11,0,90,10,0,0},// 139 - Always takes off half of the opponent's HP.
	{"Supersonic",1,4,0,55,20,12,0},// 140 - Confuses opponent.
	{"Surf",3,1,90,100,15,0,0},// 141 - Hits all adjacent Pokémon.
	{"Swift",1,1,60,200,20,0,0},// 142 - Ignores Accuracy and Evasiveness.
	{"Swords Dance",1,2,2,100,20,0,0},// 143 - Sharply raises user's Attack.
	{"Tackle",1,0,40,100,35,0,0},// 144 -  
	{"Tail Whip",1,3,1,100,30,1,0},// 145 - Lowers opponent's Defense.
	{"Take Down",1,0,90,85,20,19,0},// 146 - User receives recoil damage.
	{"Teleport",11,6,0,100,20,0,0},// 147 - Allows user to flee wild battles; also warps player to last PokéCenter.
	{"Thrash",1,0,120,100,10,32,0},// 148 - User attacks for 2-3 turns but then becomes confused.
	{"Thunder",4,1,110,70,10,9,10},// 149 - May paralyze opponent.
	{"Thunder Punch",4,0,75,100,15,9,10},// 150 - May paralyze opponent.
	{"Thunder Shock",4,1,40,100,30,9,10},// 151 - May paralyze opponent.
	{"Thunder Wave",4,4,0,90,20,9,0},// 152 - Paralyzes opponent.
	{"Thunderbolt",4,1,90,100,15,9,10},// 153 - May paralyze opponent.
	{"Toxic",8,4,0,90,10,7,0},// 154 - Badly poisons opponent.
	{"Transform",1,0,0,100,10,0,0},// 155 - User takes on the form and attacks of the opponent.  (NOT WORKING)
	{"Tri Attack",1,1,80,100,10,30,30},// 156 - May paralyze, burn or freeze opponent.
	{"Twineedle",12,0,25,100,20,23,0},// 157 - Hits twice in one turn. May poison opponent.
	{"Vice Grip",1,0,55,100,30,0,0},// 158 -  
	{"Vine Whip",5,0,45,100,25,0,0},// 159 -  
	{"Water Gun",3,1,40,100,25,0,0},// 160 -  
	{"Waterfall",3,0,80,100,15,18,0},// 161 - May cause flinching.
	{"Whirlwind",1,7,0,100,20,0,0},// 162 - In battles, the opponent switches. In the wild, the Pokémon runs.
	{"Wing Attack",10,0,60,100,35,0,0},// 163 -  
	{"Withdraw",3,2,1,100,40,1,0},// 164 - Raises user's Defense.
	{"Wrap",1,0,15,90,20,14,100},// 165 - Traps opponent, damaging them for 4-5 turns.
	{"Ricardo Punch",2,0,255,100,50,21,0 },// 166 - Absorbs HP from opponent
};