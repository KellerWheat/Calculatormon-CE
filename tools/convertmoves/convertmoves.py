import re

file = open("moves.txt","r")

text = file.read()

names = ""
output = ""

split = re.split(r'\t+', text.rstrip('\t').replace('\n','\t').replace('—','0'))

elementnamelist = ["NORMAL","FIRE","WATER","ELECTRIC","GRASS","ICE","FIGHTING","POISON","GROUND","FLYING","PSYCHIC","BUG","ROCK","GHOST","DRAGON","DARK","STEEL"]
def elementtonumber(element):
    i = 0
    for n in elementnamelist:
        i+=1
        if(n == element):
            return i
def typetonumber(attacktype):
    if(attacktype == "Physical"):
        return 0
    if(attacktype == "Special"):
        return 1
    return ""
    

for x in range(int(0),int(len(split))):
    if(x%7 == 0):
        output += '\t{"';
        output += split[x]
        output += '",';
        names += split[x]
        names += ',';
    if(x%7 == 1):
        output += str(elementtonumber(split[x]))
        output += ',';
    if(x%7 == 2):
        output += str(typetonumber(split[x]))
        output += ',';
    if(x%7 == 3):
        if(split[x] != 0):
            output += split[x]
        output += ',';
    if(x%7 == 4):
        if(split[x] == '0'):
            output += '100'
        else:
            output += split[x]
        output += ',';
    if(x%7 == 5):
        output += split[x]
        output += ',},';
    if(x%7 == 6):
        output += '// '
        output += str(int((x+1)/7))
        output += ' - '
        output += split[x]
        output += '\n'
        
print(output)
