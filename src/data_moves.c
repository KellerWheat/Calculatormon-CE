#include "data.h";

/* movetypes:
0 - physical
1 - special
2 - raise user stat (power is number of times)
3 - lower non-user stat (power is number of times)
4 - status move (power is meaningless)
5 - specific damage (power is damage)
6 - leave battle
7 - switch opponent/leave battle (switch opponent not working)
8 - recover hp (power is amount)
9 - damage is user level
10 - damage is 50-150% of user level
11 - damage is half of enemy hp
12 - haze
13 - rest
14 - disable
15 - one hit ko
16 - random move
17 - copy enemy move
18 - transform
19 - counter
20 - bide
21 - conversion
22 - substitute
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

19-26 and 31-34 and 36-38 do not need statuschance to work:

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

31 - attack first
32 - attack 2-3 turns
33 - charge attack
34 - fly/dig
35 - payday

36 - charge attack high crit
37 - charge attack defence first turn
38 - charge attack may flinch
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
	{"Bide",1,20,0,100,10,0,0},// 9 - User takes damage for two turns then strikes back double.
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
	{"Conversion",1,21,0,100,30,0,0},// 23 - Changes user's type to target's first type
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
	{"High JumpKick",7,0,130,90,10,25,0},// 60 - If it misses, the user loses half their HP.
	{"Horn Attack",1,0,65,100,25,0,0},// 61 -  
	{"Horn Drill",1,15,0,100,5,0,0},// 62 - One-Hit-KO, if it hits.
	{"Hydro Pump",3,1,110,80,5,0,0},// 63 -  
	{"Hyper Beam",1,1,150,90,5,33,0},// 64 - User must recharge next turn.
	{"Hyper Fang",1,0,80,90,15,18,10},// 65 - May cause flinching.
	{"Hypnosis",11,4,0,60,20,10,0},// 66 - Puts opponent to sleep.
	{"Ice Beam",6,1,90,100,10,11,10},// 67 - May freeze opponent.
	{"Ice Punch",6,0,75,100,15,11,10},// 68 - May freeze opponent.
	{"JumpKick",7,0,100,95,10,25,0},// 69 - If it misses, the user loses half their HP.
	{"Karate Chop",7,0,50,100,25,24,0},// 70 - High critical hit ratio.
	{"Kinesis",11,3,1,80,15,5,0},// 71 - Lowers opponent's Accuracy.
	{"Leech Life",12,0,80,100,10,21,0},// 72 - User recovers half the HP inflicted on opponent.
	{"Leech Seed",5,4,0,90,10,13,0},// 73 - Drains HP from opponent each turn.
	{"Leer",1,3,1,100,30,1,0},// 74 - Lowers opponent's Defense.
	{"Lick",14,0,30,100,30,9,19},// 75 - May paralyze opponent.
	{"Light Screen",11,4,0,100,30,16,0},// 76 - Halves damage from Special attacks for 5 turns.
	{"Lovely Kiss",1,4,0,75,10,10,0},// 77 - Puts opponent to sleep.
	{"Low Kick",7,0,45,100,20,0,0},// 78 - 
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
	{"Pay Day",1,0,40,100,20,35,100},// 89 - A small amount of money is gained after the battle resolves.
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
	{"Razor Wind",1,1,80,100,10,36,0},// 103 - Charges on first turn, attacks on second. High critical hit ratio.
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
	{"Skull Bash",1,0,130,100,10,37,0},// 118 - Raises Defense on first turn, attacks on second.
	{"Sky Attack",10,0,140,90,5,38,30},// 119 - Charges on first turn, attacks on second. May cause flinching.
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
	{"Substitute",1,22,0,100,10,0,0},// 138 - Uses HP to creates a decoy that takes hits.
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
	{"Transform",1,18,0,100,10,0,0},// 155 - User takes on the form and attacks of the opponent.
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

char[][] data_moveDescriptions = { 'An attack that absorbs half the damage it inflicted to restore HP', 'The foe is sprayed with a harsh, hide-melting acid that may lower DEFENSE', 'The user alters its cells to liquefy itself and sharply raise DEFENSE', 'The user relaxes and lightens its body to sharply boost its SPEED', 'The user temporarily empties its mind to forget its concerns. It sharply raises the user’s Sp. Def stat', 'A rainbow-colored attack beam. It may lower the foe’s ATTACK stat', 'Round objects are hurled at the foe to strike two to five times', 'The user creates a sturdy wall that sharply raises its DEFENSE stat', 'The user endures attacks for two turns, then strikes back double', 'A long body or tentacles are used to bind the foe for two to five turns', 'The user bites with vicious fangs. It may make the foe flinch', 'The foe is blasted with a blizzard. It may freeze the foe solid', 'The user drops its full body on the foe. It may leave the foe paralyzed', 'The foe is clubbed with a bone held in hand. It may make the foe flinch', 'The user throws a bone that hits the foe once, then once again on return', 'A spray of bubbles hits the foe. It may lower the foe’s SPEED stat', 'A spray of bubbles strikes the foe. It may lower the foe’s SPEED stat', 'The foe is clamped and squeezed by the user’s shell for two to five turns', 'The foe is hit with a flurry of punches that strike two to five times', 'The foe is exposed to a sinister ray that triggers confusion', 'A weak telekinetic attack that may also leave the foe confused', 'The foe is attacked with long tentacles or vines. It may lower SPEED', 'The user changes its type to match the type of one of its moves', 'A retaliation move that counters any physical hit with double the damage', 'A large pincer is used to hammer the foe. It has a high critical-hit ratio', 'A basic attack. It can be used to cut down thin trees and grass', 'The user curls up to conceal weak spots and raise its DEFENSE stat', 'An attack that hits on the 2nd turn. Can also be used to exit dungeons', 'For a few turns, it prevents the foe from using the move it last used', 'The foe is hit with a rhythmic punch that may leave it confused', 'Two legs are used to quickly kick the foe twice in one turn', 'The foe is slapped repeatedly, back and forth, two to five times', 'The user creates illusory copies of itself to raise its evasiveness', 'A reckless, life-risking tackle that also hurts the user a little', 'The foe is hit with a shock wave that always inflicts 40-HP damage', 'Absorbs half the damage it inflicted on a sleeping foe to restore HP', 'A corkscrewing attack with the sharp beak acting as a drill', 'An earthquake that strikes all POKéMON in battle excluding the user', 'A large egg is hurled with great force at the foe to inflict damage', 'The foe is attacked with small flames. The foe may suffer a burn', 'The user explodes to inflict terrible damage even while fainting itself', 'The foe is hit with an intense flame. It may leave the target with a burn', 'The foe is punched with a fiery fist. It may leave the foe with a burn', 'The foe is trapped in an intense spiral of fire that rages two to five turns', 'The foe is dropped into a fissure. The foe faints if it hits', 'The foe is scorched with intense flames. The foe may suffer a burn', 'A blast of light that cuts the foe’s accuracy. It also illuminates caves', 'A 2-turn move that hits on the 2nd turn. Use it to fly to any known town', 'The user takes a deep breath and focuses to raise its critical-hit ratio', 'The foe is jabbed repeatedly with a horn or beak two to five times', 'The foe is raked with sharp claws or scythes two to five times', 'The user intimidates the foe with the design on its belly to cause paralysis', 'The user growls in a cute way, making the foe lower its ATTACK stat', 'The user’s body is forced to grow, raising the SP. ATK stat', 'A vicious tearing attack with pincers. The foe will faint if it hits', 'A gust of wind is whipped up by wings and launched at the foe to inflict damage.', 'The user stiffens all the muscles in its body to raise its DEFENSE stat', 'Eliminates all stat changes among all POKéMON engaged in battle', 'The user sticks its head out and rams. It may make the foe flinch', 'A strong jumping knee kick. If it misses, the user is hurt', 'The foe is jabbed with a sharply pointed horn to inflict damage', 'The horn is rotated like a drill to ram. The foe will faint if it hits', 'A high volume of water is blasted at the foe under great pressure', 'A severely damaging attack that makes the user rest on the next turn', 'The foe is attacked with sharp fangs. It may make the foe flinch', 'Hypnotic suggestion is used to make the foe fall into a deep sleep', 'The foe is struck with an icy beam. It may freeze the foe solid', 'The foe is punched with an icy fist. It may leave the foe frozen', 'The user jumps up high, then kicks. If it misses, the user hurts itself', 'The foe is attacked with a sharp chop. It has a high critical-hit ratio', 'The user distracts the foe by bending a spoon. It may lower accuracy', 'An attack that absorbs half the damage it inflicted to restore HP', 'A seed is planted on the foe to steal some HP for the user on every turn', 'The foe is given an intimidating look that lowers its DEFENSE stat', 'The foe is licked and hit with a long tongue. It may also paralyze', 'A wall of light cuts damage from SP. ATK attacks for five turns', 'The user forces a kiss on the foe with a scary face that induces sleep', 'A low, tripping kick that inflicts more damage on heavier foes', 'The user meditates to awaken its power and raise its ATTACK stat', 'A tough attack that drains half the damage it inflicted to restore HP', 'The foe is attacked by a kick fired with muscle-packed power', 'The foe is slugged by a punch thrown with muscle-packed power', 'Waggles a finger and stimulates the brain into using any move at random', 'The user copies the move last used by the foe for the rest of the battle', 'The user compresses all the cells in its body to raise its evasiveness', 'The user counters the move last used by the foe with the same move', 'The ally party is protected by a mist that prevents stat reductions', 'An attack with a mirage that inflicts damage matching the user’s level', 'Numerous coins are hurled at the foe. Money is earned after battle', 'The foe is jabbed with a sharply pointed beak or horn', 'The user attacks with petals for two to three turns, then gets confused', 'Sharp pins are shot at the foe and hit two to five times at once', 'The foe is sprayed with a cloud of toxic gas that may poison the foe', 'A cloud of toxic dust is scattered. It may poison the foe', 'The foe is stabbed with a toxic barb, etc. It may poison the foe', 'A physical attack delivered with a long tail or a foreleg, etc', 'A peculiar ray is shot at the foe. It may leave the foe confused', 'A strong telekinetic attack. It may also lower the foe’s SP. DEF stat', 'The foe is attacked with an odd, hot energy wave that varies in intensity', 'An almost invisibly fast attack that is certain to strike first', 'An attack that becomes stronger each time the user is hit in battle', 'The foe is hit with a cutting leaf. It has a high critical-hit ratio', 'Blades of wind hit the foe on the 2nd turn. It has a high critical-hit ratio', 'A self-healing move that restores HP by up to half of the user’s maximum HP', 'A wall of light cuts damage from physical attacks for five turns', 'The user sleeps for two turns to fully restore HP and heal any status problem', 'The foe is made to switch out with an ally. In the wild, the battle ends', 'Large boulders are hurled at the foe. It may make the foe flinch', 'The foe is attacked with a shower of small, easily thrown rocks', 'A quick kick from a rolling spin. It may make the foe flinch', 'A lot of sand is hurled in the foe’s face, reducing its accuracy', 'Hard, pointed, and sharp claws rake the foe', 'An ear-splitting screech is emitted to sharply reduce the foe’s DEFENSE', 'A gravity-fed throw that causes damage matching the user’s level', 'The user blows up to inflict severe damage, even making itself faint', 'The user reduces its polygon count to sharpen edges and raise ATTACK', 'A soothing song in a calming voice lulls the foe into a deep slumber', 'The user raises its DEFENSE in the 1st turn, then attacks in the 2nd turn', 'A 2nd-turn attack move with a high critical-hit ratio. The foe may flinch', 'The foe is struck with a long tail, vines, etc', 'The foe is slashed with claws, etc. It has a high critical-hit ratio', 'A sleep-inducing dust is scattered in high volume around a foe', 'Toxic sludge is hurled at the foe. It may poison the target', 'The foe is attacked with exhaust gases. It may also poison the foe', 'An obscuring cloud of smoke or ink reduces the foe’s accuracy', 'Heals the user by up to half its full HP. It can be used to heal an ally', 'A 2-turn move that blasts the foe with absorbed energy in the 2nd turn', 'The foe is hit with a shock wave that always inflicts 20-HP damage', 'Sharp spikes are fired at the foe to strike two to five times', 'The user just flops and splashes around without having any effect', 'The user scatters bursts of fine spores that induce sleep', 'The foe is stomped with a big foot. It may make the foe flinch', 'The foe is slugged at maximum power. Can also be used to move boulders', 'The foe is bound with strings shot from the mouth to reduce its SPEED', 'An attack that is used only if there is no PP. It also hurts the user', 'Paralyzing dust is scattered wildly. It may paralyze the foe', 'A reckless, full-body throw attack that also hurts the user a little', 'The user creates a decoy using one-quarter of its full HP', 'The user attacks with sharp fangs and halves the foe’s HP', 'The user generates odd sound waves. It may confuse the foe', 'A big wave crashes down on the foe. Can also be used for crossing water', 'Star-shaped rays that never miss are fired at all foes in battle', 'A frenetic dance of fighting. It sharply raises the ATTACK stat', 'A physical attack in which the user charges, full body, into the foe', 'The user wags its tail cutely, making the foe lower its DEFENSE stat', 'A reckless, full-body charge attack that also hurts the user a little', 'Use it to flee from any wild POKéMON. Also warps to the last POKé CENTER', 'The user rampages about for two to three turns, then becomes confused', 'A brutal lightning attack that may also leave the foe paralyzed', 'The foe is punched with an electrified fist. It may leave the foe paralyzed', 'An electric shock attack that may also leave the foe paralyzed', 'A weak electric shock that is sure to cause paralysis if it hits', 'A strong electrical attack that may also leave the foe paralyzed', 'A move that badly poisons the foe. Its poison damage worsens every turn', 'The user transforms into a copy of the foe with even the same move set', 'A simultaneous 3-beam attack that may paralyze, burn, or freeze the foe', 'The foe is stabbed twice with foreleg stingers. It may poison the foe', 'Huge, impressive pincers grip and squeeze the foe', 'The foe is struck with slender, whiplike vines', 'The foe is struck with a lot of water expelled forcibly from the mouth', 'A powerful charge attack. It can also be used to climb a waterfall', 'The foe is made to switch out with an ally. In the wild, the battle ends', 'The foe is struck with large, imposing wings spread wide', 'The user withdraws its body in its hard shell, raising its DEFENSE stat', 'A long body or vines are used to wrap the foe for two to five turns' }