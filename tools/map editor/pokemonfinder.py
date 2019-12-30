import os
import numpy as np

def find():
    pokemon = []
    for x in range(0,151):
    	pokemon += [[]]
    areas = []
    for x in range(0,32):
    	areas += [[]]
    
    names = "None,Bulbasaur,Ivysaur,Venusaur,Charmander,Charmeleon,Charizard,Squirtle,Wartortle,Blastoise,Caterpie,Metapod,Butterfree,Weedle,Kakuna,Beedrill,Pidgey,Pidgeotto,Pidgeot,Rattata,Raticate,Spearow,Fearow,Ekans,Arbok,Pikachu,Raichu,Sandshrew,Sandslash,Nidoran(F),Nidorina,Nidoqueen,Nidoran(M),Nidorino,Nidoking,Clefairy,Clefable,Vulpix,Ninetales,Jigglypuff,Wigglytuff,Zubat,Golbat,Oddish,Gloom,Vileplume,Paras,Parasect,Venonat,Venomoth,Diglett,Dugtrio,Meowth,Persian,Psyduck,Golduck,Mankey,Primeape,Growlithe,Arcanine,Poliwag,Poliwhirl,Poliwrath,Abra,Kadabra,Alakazam,Machop,Machoke,Machamp,Bellsprout,Weepinbell,Victreebel,Tentacool,Tentacruel,Geodude,Graveler,Golem,Ponyta,Rapidash,Slowpoke,Slowbro,Magnemite,Magneton,Farfetch'd,Doduo,Dodrio,Seel,Dewgong,Grimer,Muk,Shellder,Cloyster,Gastly,Haunter,Gengar,Onix,Drowzee,Hypno,Krabby,Kingler,Voltorb,Electrode,Exeggcute,Exeggutor,Cubone,Marowak,Hitmonlee,Hitmonchan,Lickitung,Koffing,Weezing,Rhyhorn,Rhydon,Chansey,Tangela,Kangaskhan,Horsea,Seadra,Goldeen,Seaking,Staryu,Starmie,Mr. Mime,Scyther,Jynx,Electabuzz,Magmar,Pinsir,Tauros,Magikarp,Gyarados,Lapras,Ditto,Eevee,Vaporeon,Jolteon,Flareon,Porygon,Omanyte,Omastar,Kabuto,Kabutops,Aerodactyl,Snorlax,Articuno,Zapdos,Moltres,Dratini,Dragonair,Dragonite,Mewtwo,Mew,".split(',')
    areaCount = 0
    
    for x in range(0,32):
        if(os.path.isfile("zonedata_" + str(x) + ".npy")):
            areaCount +=1
            zonedata = np.load("zonedata_" + str(x) + ".npy")
            for y in range(0,5):
                for z in range(0,16):
                    if(x not in pokemon[zonedata[16*y+z]]):
                        pokemon[zonedata[16*y+z]] += [x]
                    if(zonedata[16*y+z] not in areas[x]):
                        areas[x] += [zonedata[16*y+z]]
    
    for x in range(0,151):
        print(names[x] + ": " + str(pokemon[x]).replace('[','').replace(']',''))
    for x in range(0,areaCount):
        text = str(x) + ": "
        for t in areas[x]:
            text += names[t] + ","
        print(text)
def findT():
    pokemon = []
    for x in range(0,151):
    	pokemon += [[]]
    areas = []
    for x in range(0,32):
    	areas += [[]]
    
    names = "None,Bulbasaur,Ivysaur,Venusaur,Charmander,Charmeleon,Charizard,Squirtle,Wartortle,Blastoise,Caterpie,Metapod,Butterfree,Weedle,Kakuna,Beedrill,Pidgey,Pidgeotto,Pidgeot,Rattata,Raticate,Spearow,Fearow,Ekans,Arbok,Pikachu,Raichu,Sandshrew,Sandslash,Nidoran(F),Nidorina,Nidoqueen,Nidoran(M),Nidorino,Nidoking,Clefairy,Clefable,Vulpix,Ninetales,Jigglypuff,Wigglytuff,Zubat,Golbat,Oddish,Gloom,Vileplume,Paras,Parasect,Venonat,Venomoth,Diglett,Dugtrio,Meowth,Persian,Psyduck,Golduck,Mankey,Primeape,Growlithe,Arcanine,Poliwag,Poliwhirl,Poliwrath,Abra,Kadabra,Alakazam,Machop,Machoke,Machamp,Bellsprout,Weepinbell,Victreebel,Tentacool,Tentacruel,Geodude,Graveler,Golem,Ponyta,Rapidash,Slowpoke,Slowbro,Magnemite,Magneton,Farfetch'd,Doduo,Dodrio,Seel,Dewgong,Grimer,Muk,Shellder,Cloyster,Gastly,Haunter,Gengar,Onix,Drowzee,Hypno,Krabby,Kingler,Voltorb,Electrode,Exeggcute,Exeggutor,Cubone,Marowak,Hitmonlee,Hitmonchan,Lickitung,Koffing,Weezing,Rhyhorn,Rhydon,Chansey,Tangela,Kangaskhan,Horsea,Seadra,Goldeen,Seaking,Staryu,Starmie,Mr. Mime,Scyther,Jynx,Electabuzz,Magmar,Pinsir,Tauros,Magikarp,Gyarados,Lapras,Ditto,Eevee,Vaporeon,Jolteon,Flareon,Porygon,Omanyte,Omastar,Kabuto,Kabutops,Aerodactyl,Snorlax,Articuno,Zapdos,Moltres,Dratini,Dragonair,Dragonite,Mewtwo,Mew,".split(',')
    areaCount = 0
    
    for x in range(0,32):
        if(os.path.isfile("zonedata_" + str(x) + ".npy")):
            areaCount +=1
            zonedata = np.load("zonedata_" + str(x) + ".npy")
            for y in range(0,6):
                for z in range(0,16):
                    if(x not in pokemon[zonedata[16*(23+y)+z]]):
                        pokemon[zonedata[16*(23+y)+z]] += [x]
                    if(zonedata[16*(23+y)+z] not in areas[x]):
                        areas[x] += [zonedata[16*(23+y)+z]]
    
    for x in range(0,151):
        print(names[x] + ": " + str(pokemon[x]).replace('[','').replace(']',''))
    for x in range(0,areaCount):
        text = str(x) + ": "
        for t in areas[x]:
            text += names[t] + ","
        print(text)
find()
