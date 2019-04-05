
import re
from bs4 import BeautifulSoup
import requests
import urllib

output = ['']*151

characternames = ''

#Functions

elementnamelist = ["NORMAL","FIRE","WATER","ELECTRIC","GRASS","ICE","FIGHTING","POISON","GROUND","FLYING","PSYCHIC","BUG","ROCK","GHOST","DRAGON","DARK","STEEL"]
def elementtonumber(element):
    index = 0
    for n in elementnamelist:
        index+=1
        if(n.lower().replace(' ','').replace('-','') == element.lower().replace(' ','').replace('-','')):
            return index
    return 0

movenames = 'Absorb,Acid,Acid Armor,Agility,Amnesia,Aurora Beam,Barrage,Barrier,Bide,Bind,Bite,Blizzard,Body Slam,Bone Club,Bonemerang,Bubble,Bubble Beam,Clamp,Comet Punch,Confuse Ray,Confusion,Constrict,Conversion,Counter,Crabhammer,Cut,Defense Curl,Dig,Disable,Dizzy Punch,Double Kick,Double Slap,Double Team,Double-Edge,Dragon Rage,Dream Eater,Drill Peck,Earthquake,Egg Bomb,Ember,Explosion,Fire Blast,Fire Punch,Fire Spin,Fissure,Flamethrower,Flash,Fly,Focus Energy,Fury Attack,Fury Swipes,Glare,Growl,Growth,Guillotine,Gust,Harden,Haze,Headbutt,High Jump Kick,Horn Attack,Horn Drill,Hydro Pump,Hyper Beam,Hyper Fang,Hypnosis,Ice Beam,Ice Punch,Jump Kick,Karate Chop,Kinesis,Leech Life,Leech Seed,Leer,Lick,Light Screen,Lovely Kiss,Low Kick,Meditate,Mega Drain,Mega Kick,Mega Punch,Metronome,Mimic,Minimize,Mirror Move,Mist,Night Shade,Pay Day,Peck,Petal Dance,Pin Missile,Poison Gas,Poison Powder,Poison Sting,Pound,Psybeam,Psychic,Psywave,Quick Attack,Rage,Razor Leaf,Razor Wind,Recover,Reflect,Rest,Roar,Rock Slide,Rock Throw,Rolling Kick,Sand Attack,Scratch,Screech,Seismic Toss,Self-Destruct,Sharpen,Sing,Skull Bash,Sky Attack,Slam,Slash,Sleep Powder,Sludge,Smog,Smokescreen,Soft-Boiled,Solar Beam,Sonic Boom,Spike Cannon,Splash,Spore,Stomp,Strength,String Shot,Struggle,Stun Spore,Submission,Substitute,Super Fang,Supersonic,Surf,Swift,Swords Dance,Tackle,Tail Whip,Take Down,Teleport,Thrash,Thunder,Thunder Punch,Thunder Shock,Thunder Wave,Thunderbolt,Toxic,Transform,Tri Attack,Twineedle,Vice Grip,Vine Whip,Water Gun,Waterfall,Whirlwind,Wing Attack,Withdraw,Wrap'
def movetoindex(name):
    index = 0
    for s in movenames.split(','):
        if(s.lower().replace(' ','')==name.lower().replace(' ','')):
            return(index+1)
        index+=1
        
def visible(element):
    if element.parent.name in ['style', 'script', '[document]', 'head', 'title']:
        return False
    elif re.match('<!--.*-->', str(element.encode('utf-8'))):
        return False
    return True


#Name and elements

file = open("charactertypes.txt","r")
text = file.read()

split1 = text.split('\n')

for i in range(0,151):
    split2 = re.split(r'\t+', split1[i].rstrip('\t'))
    output[i] += '{"'
    output[i] += split2[3]
    characternames += split2[3] + ','
    output[i] += '",'
    output[i] += str(elementtonumber(split2[4]))
    output[i] += ','
    if(len(split2)==6):
        output[i] += str(elementtonumber(split2[5]))
    else:
        output[i] += '0'
    output[i] += ','

#Base Stats

file = open("basestats.txt","r")
text = file.read()

split1 = text.split('\n')

for i in range(0,151):
    split2 = re.split(r'\t+', split1[i].rstrip('\t'))
    output[i] += split2[3] + ','
    output[i] += split2[4] + ','
    output[i] += split2[5] + ','
    output[i] += split2[6] + ','
    output[i] += split2[7] + ','
    output[i] += split2[8] + ','

#Moves


file = open("learnsets.txt","r")
text = file.read()

split1 = text.split('\n')

index1 = 0
index2 = 0

buffer1=[0]*10
buffer2=[0]*10

for i in range(0,len(split1)):
    if(len(split1[i])>0):
        if(split1[i][0]=='#' and i != 0):
            output[index1] += '{' + str(buffer1).replace('[','').replace(']','').replace("'","") + '},{' + str(buffer2).replace('[','').replace(']','').replace("'","") + '},'
            index1 += 1
            index2 = 0
            buffer1=[0]*10
            buffer2=[0]*10
        if(split1[i][0]=='S'):
            buffer1[index2] = 0
            buffer2[index2] = movetoindex(split1[i].split(' - ')[1].split(' (')[0])
            index2 += 1
        if(split1[i][0]=='L'):
            buffer1[index2] = split1[i].split()[1]
            buffer2[index2] = movetoindex(split1[i].split(' - ')[1].split(' (')[0])
            index2 += 1

#Evolution

file = open("evolutions.txt","r")
text = file.read()

split1 = text.split('\n')

evolvesfrom = [0]*151
for i in range(0,151):
    split2 = split1[i].split()
    if(len(split2)>1):
        output[i] += split2[1] + ',' + split2[2] + ','
        evolvesfrom[int(split2[2])-1] = i+1
    else:
        output[i] += '0,0,'
for i in range(0,151):
    output[i] += str(evolvesfrom[i]) + ','

#BaseXP

file = open("xpdrops.txt","r")
text = file.read()

split1 = text.split('\n')

for i in range(0,151):
    split2 = re.split(r'\t+', split1[i].rstrip('\t'))
    output[i] += split2[3] + ','

#Xp Type

file = open("xptypes.txt","r")
text = file.read()

split1 = text.split('\n')

for i in range(0,151):
    split2 = re.split(r'\t+', split1[i].rstrip('\t'))
    if(split2[3]=='Slow'):
        output[i] += '0,'
    if(split2[3]=='Medium Slow'):
        output[i] += '1,'
    if(split2[3]=='Medium Fast'):
        output[i] += '2,'
    if(split2[3]=='Fast'):
        output[i] += '3,'

#Catch Rate

file = open("catchrates.txt","r")
text = file.read()

split1 = text.split('\n')

for i in range(0,151):
    split2 = re.split(r'\t+', split1[i].rstrip('\t'))
    output[i] += split2[3]

#Output

outputtext = ''
for i in range(0,151):
    outputtext += output[i].replace(' ','') + '},// ' + str(i+1) + '\n\t'
print(outputtext)
