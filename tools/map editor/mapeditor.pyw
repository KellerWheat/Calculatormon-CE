from tkinter import *
import numpy as np
import os
import pygame
from PIL import Image
import sys
import pyperclip


#--------
#Settings
#--------
Setting_SizeX = 48
Setting_SizeY = 40
Setting_BSizeX = 21
Setting_BSizeY = 15
tilesetname = 'tileset.png'
indoortilesetname = 'indoortileset.png'
dataCount = 40
trainerTextCount = 32
npcCount = 32

#------------
#Current Data
#------------

sizeX = 1
sizeY = 1
tilemap = np.array([0])
typemap = np.array([0])
zonedata = np.array([0])
fileName = ""
tiles = [None]
editMode = 0
paintMode = 0
numberToPaint = 0
paintWidth = 1
paintHeight = 1

#-------------
#Start Tkinter
#-------------

root = Tk()
root.title('Map Editor')
root.wm_state('zoomed')

#---------------------------
#Load Trainer/NPC Text Lists
#---------------------------


trainerText = np.array(["                                "])
npcText = np.array(["                                "])
npcReward = np.array([0])

trainerText[0] = ""
npcText[0] = ""

if(os.path.isfile("trainertext.npy")):
    trainerText = np.load('trainertext.npy')
if(os.path.isfile("npctext.npy")):
    npcText = np.load('npctext.npy')
if(os.path.isfile("npcrewards.npy")):    
    npcReward = np.load('npcrewards.npy')

trainerText = np.resize(trainerText, (trainerTextCount))
npcText = np.resize(npcText, (npcCount))
npcReward = np.resize(npcReward, (npcCount))

currentTrainer = IntVar()
currentNpc = IntVar()
Var_TrainerText = StringVar()
Var_NpcText = StringVar()
Var_NpcReward = IntVar()
Var_TrainerText.set(trainerText[0])
Var_NpcText.set(npcText[0])
Var_NpcReward.set(npcReward[0])

def UpdateTrainer(new):
    trainerText[currentTrainer.get()] = Var_TrainerText.get()
    currentTrainer.set(new)
    Var_TrainerText.set(trainerText[currentTrainer.get()])
    np.save('trainertext.npy', trainerText)

def UpdateNpc(new):
    npcText[currentNpc.get()] = Var_NpcText.get()
    npcReward[currentNpc.get()] = Var_NpcReward.get()
    currentNpc.set(new)
    Var_NpcText.set(npcText[currentNpc.get()])
    Var_NpcReward.set(npcReward[currentNpc.get()])
    np.save('npctext.npy',npcText)
    np.save('npcrewards.npy',npcReward)

UpdateTrainer(0)
UpdateNpc(0)


#---------
#SelectMap
#---------

def SetupTileMap():
    global tiles, tilesetimage, tilesetname, indoortilesetname
    if(Var_IsBuilding.get()==1):
        tileset = Image.open(indoortilesetname)
    else:
        tileset = Image.open(tilesetname)
    tilesetimage = pygame.image.fromstring(tileset.tobytes(),tileset.size,tileset.mode)
    tilex,tiley = tileset.size
    tilex=int(tilex/16)
    tiley=int(tiley/16)
    tilecount = tilex*tiley
    tiles = [None] * tilecount
    for x in range(tilex):
        for y in range(tiley):
            image = tileset.crop((x*16,y*16,x*16+16,y*16+16))
            tiles[tilex*y+x] = pygame.image.fromstring(image.tobytes(),image.size,image.mode)

def LoadVars():
    pass
def SaveMap():
    global tilemap, typemap, zonedata, fileName
    SaveVars()
    np.save("tilemap" + fileName,tilemap)
    np.save("typemap" + fileName,typemap)
    np.save("zonedata" + fileName,zonedata)

def ChangeMap():
    global tilemap, typemap, zonedata, sizeX, sizeY, fileName
    Var_Current.set("Editing " + str(Var_MapIndex.get()) + ("B" if (Var_IsBuilding.get()==1) else ""))
    if((Var_IsBuilding.get()==1)):
        sizeX = Setting_BSizeX
        sizeY = Setting_BSizeY
        fileName = ("_B" + str(Var_MapIndex.get()) + ".npy")
    else:
        sizeX = Setting_SizeX
        sizeY = Setting_SizeY
        fileName = ("_" + str(Var_MapIndex.get()) + ".npy")
    if(os.path.isfile("tilemap" + fileName)):
        tilemap = np.load("tilemap" + fileName)
    else:
        tilemap = np.array([0]*(sizeX*sizeX))
    if(os.path.isfile("typemap" + fileName)):
        typemap = np.load("typemap" + fileName)
    else:
        typemap = np.array([0]*(sizeX*sizeX))
    if(os.path.isfile("zonedata" + fileName)):
        zonedata = np.load("zonedata" + fileName)
    else:
        zonedata = np.array([0]*(dataCount * 16))

    tilemap.resize(sizeX*sizeY)
    typemap.resize(sizeX*sizeY)
    zonedata.resize(dataCount * 16)
    
    LoadVars()
    SetupTileMap()

def CopyAll():
    UpdateTrainer(currentTrainer.get())
    UpdateNpc(currentNpc.get())
    text = ""
    text += "char data_trainerText["+str(trainerTextCount)+"][32] = {\n\t"
    for i in range(0,trainerTextCount):
        text += '"'+trainerText[i]+'",'
    text += "\n};\n"
    text += "char data_npcText["+str(npcCount)+"][32] = {\n\t"
    for i in range(0,npcCount):
        text += '"'+npcText[i]+'",'
    text += "\n};\n"
    text += "uint8_t data_npcReward["+str(npcCount)+"] = {\n\t"
    for i in range(0,npcCount):
        text += str(npcReward[i])+','
    text += "\n};\n"
    
    pyperclip.copy(text)

def ExportFile():
    output0 = np.array([])
    output1 = np.array([])
    for i in range(0, Var_ZoneCount.get()):
        output0 = np.append(output0, np.load('tilemap_' + str(i) + '.npy'))
        output0 = np.append(output0, np.load('typemap_' + str(i) + '.npy'))
        output0 = np.append(output0, np.load('zonedata_' + str(i) + '.npy'))
    for i in range(0, Var_BuildingCount.get()):
        output1 = np.append(output1, np.load('tilemap_B' + str(i) + '.npy'))
        output1 = np.append(output1, np.load('typemap_B' + str(i) + '.npy'))
        output1 = np.append(output1, np.load('zonedata_B' + str(i) + '.npy'))
    #with open('PKMNMD0.bin', 'wb') as f:
    #    f.write(output0.astype('B'))
    #with open('PKMNMD1.bin', 'wb') as f:
    #    f.write(output1.astype('B'))
    output0.astype('B').tofile('PKMNMD0.bin')
    print(output0.astype('B'))
    output1.astype('B').tofile('PKMNMD1.bin')
    os.startfile("convertmaps.bat")
    
        

Canvas_SetMap = Frame(root)
Canvas_SetMap.grid(row=0,column=0, padx=16, sticky=N)

Var_Current = StringVar()
Label_Current = Label(Canvas_SetMap,textvar=Var_Current)
Label_Current.grid(row=0,column=0)
Button_SaveMap = Button(Canvas_SetMap,text="Save Map", command=SaveMap)
Button_SaveMap.grid(row=1,column=0)

Canvas_SetMap.grid_rowconfigure(2, minsize=20)

Var_MapIndex = IntVar()
Label_MapIndex = Label(Canvas_SetMap,text="Map Index: ")
Label_MapIndex.grid(row=3,column=0)
Input_MapIndex = Entry(Canvas_SetMap,textvariable=Var_MapIndex)
Input_MapIndex.grid(row=4,column=0)

Var_IsBuilding = IntVar()

Input_IsBuilding = Checkbutton(Canvas_SetMap,variable=Var_IsBuilding,text="Building")
Input_IsBuilding.grid(row=6,column=0)

Button_ChangeMap = Button(Canvas_SetMap,text="Change Map", command=ChangeMap)
Button_ChangeMap.grid(row=7,column=0)

Canvas_SetMap.grid_rowconfigure(8, minsize=20)

Var_ZoneCount = IntVar()
Label_ZoneCount = Label(Canvas_SetMap,text="Zone Count: ")
Label_ZoneCount.grid(row=9,column=0)
Input_ZoneCount = Entry(Canvas_SetMap,textvariable=Var_ZoneCount)
Input_ZoneCount.grid(row=10,column=0)

Var_BuildingCount = IntVar()
Label_BuildingCount = Label(Canvas_SetMap,text="Building Count: ")
Label_BuildingCount.grid(row=11,column=0)
Input_BuildingCount = Entry(Canvas_SetMap,textvariable=Var_BuildingCount)
Input_BuildingCount.grid(row=12,column=0)

Button_CopyAll = Button(Canvas_SetMap,text="Copy All", command=CopyAll)
Button_CopyAll.grid(row=13,column=0)
Button_CopyAll = Button(Canvas_SetMap,text="Export Data", command=ExportFile)
Button_CopyAll.grid(row=14,column=0)

ChangeMap()


#-----------
#Display Map
#-----------

TypeNames = [' ','BE','C','N','N','N','N','N','N','N','N','N','N','N','N','N','G0','G1','G2','G3','G4','G5','G6','G7','G8','G9','GA','GB','GC','GD','GE','GF','E0','E1','E2','E3','E4','E5','E6','E7','E8','E9','EA','EB','EC','ED','EE','EF','V0','V1','V2','V3','V4','V5','V6','V7','V8','V9','VA','VB','VC','VD','VE','VF','W','H','S','B','N','N','N','N','N','N','N','N','N','N','N','N','D0','D1','D2','D3','D4','D5','D6','D7','D8','D9','DA','DB','DC','DD','DE','DF','T0','T1','T2','T3','T4','T5','T6','T7','T8','T9','TA','TB','TC','TD','TE','TF','B0','B1','B2','B3','B4','B5','B6','B7','B8','B9','BA','BB','BC','BD','BE','BF',]

SetupTileMap()

Frame_DisplayMap = Frame(root, width=(16*Setting_SizeX) + 272, height=(16*Setting_SizeY))
Frame_DisplayMap.grid(row=0,column=2, padx=16)
root.update()
os.environ['SDL_WINDOWID'] = str(Frame_DisplayMap.winfo_id())
os.environ['SDL_VIDEODRIVER'] = 'windib'
pygame.init()
screen = pygame.display.set_mode((16*Setting_SizeX + 272,16*Setting_SizeY))
pygame.display.flip()
tilefont = pygame.font.SysFont('comicsansms', 10)
infofont = pygame.font.SysFont('comicsansms', 30)
def draw():
    global txtemp, tytemp, numberToPaint, paintWidth, paintHeight, sizeX, sizeY, editMode
    events = pygame.event.get()
    
    keys = pygame.key.get_pressed()
    if keys[pygame.K_LSHIFT]:
        shift=True
    else:
        shift=False 
    
    screen.fill(pygame.Color(240,240,240))
    mouse = pygame.mouse.get_pos()
    tx = int(mouse[0]/16)
    ty = int(mouse[1]/16)
    
    if tx<sizeX and ty<sizeY :
        screen.blit(infofont.render(str(tx)+","+str(ty), True, (0, 0, 0)),((sizeX*16)+10,-5))
        if pygame.mouse.get_pressed()[0]:
            if editMode == 0:
                for x in range(0,paintWidth):
                    for y in range(0,paintHeight):
                        tilemap[tx+x+sizeX*(ty+y)] = numberToPaint+x+16*y
                        
            else:
                typemap[tx+sizeX*ty] = numberToPaint
    
    for x in range(sizeX):
        for y in range(sizeY):
            screen.blit(tiles[tilemap[x+sizeX*y]],(16*x,16*y))
            if editMode > 0:
                  mpdisplay = tilefont.render(TypeNames[typemap[x+sizeX*y]], True, (0, 0, 0))#str(format(typemap[x+sizeX*y], '02X')), True, (0, 0, 0))
                  screen.blit(mpdisplay,(16*x,16*y))
    if editMode == 0:
        screen.blit(tilesetimage,((sizeX*16)+16,112))
        pygame.draw.rect(screen,(0,0,160),(((sizeX*16)+16+(numberToPaint % 16)*16),(112+int(numberToPaint/16)*16),16*paintWidth,16*paintHeight),1)
        if(tx > (sizeX) and ty > 6 and ty < 15):
            if pygame.mouse.get_pressed()[0]:
                if(shift):
                    paintWidth = 1+tx-txtemp
                    paintHeight = 1+ty-tytemp   
                else:
                    numberToPaint = (tx - (sizeX+1))+(16*(ty-7))
                    paintWidth = 1
                    paintHeight = 1
                    txtemp = tx
                    tytemp = ty
        

    pygame.display.flip()

#-----------
#Edit Data
#-----------



ModeList = ["Tiles","Ground Types","Grass","Exit","Trainer View","Door","Trainer","Need Badges"]
TypeList = ["Ground","Building Exit","Wall","Healing","Shop","Box","NPC1 (indoors)","NPC2 (indoors)","Coast"]
TypeNumbers = [0,1,64,65,66,67,68,69,2]
def CalculateNumberToPaint():
    global editMode, numberToPaint, paintWidth, paintHeight
    Frame_ChangeNumber.grid_forget()
    Frame_ChangeType.grid_forget()
    for i in range(0,4):
        DataFrames[i].grid_forget()
    if(editMode>1):
        Frame_ChangeNumber.grid(row=0,column=1)
    if(editMode==0):
        numberToPaint = 0
        paintWidth = 1
        paintHeight = 1
    if(editMode==1):
        Frame_ChangeType.grid(row=0,column=1)
        for i in range(0, len(TypeList)):
            if(Var_Type.get()==TypeList[i]):
                numberToPaint = TypeNumbers[i]
    if(editMode==2):
        numberToPaint = 16 + Var_Number.get()
        DataFrames[0].grid(row=1,column=0,sticky=N)
    if(editMode==3):
        numberToPaint = 32 + Var_Number.get()
        DataFrames[1].grid(row=1,column=0,sticky=N)
    if(editMode==4):
        numberToPaint = 48 + Var_Number.get()
        DataFrames[3].grid(row=1,column=0,sticky=N)
    if(editMode==5):
        numberToPaint = 80 + Var_Number.get()
        DataFrames[2].grid(row=1,column=0,sticky=N)
    if(editMode==6):
        numberToPaint = 96 + Var_Number.get()
        DataFrames[3].grid(row=1,column=0,sticky=N)
    if(editMode==7):
        numberToPaint = 112 + Var_Number.get()

def ChangeMode(value):
    global editMode
    for i in range(0, len(ModeList)):
        if(Var_Mode.get()==ModeList[i]):
            editMode = i
            SwitchNumber(0)
            CalculateNumberToPaint()
            
def Decrease():
    if(Var_Number.get()-1==-1):
        SwitchNumber(15)
    else:
        SwitchNumber(Var_Number.get()-1)
    CalculateNumberToPaint()
def Increase():
    if(Var_Number.get()+1==16):
        SwitchNumber(0)
    else:
        SwitchNumber(Var_Number.get()+1)
    CalculateNumberToPaint()

Frame_DataEditor = Frame(root)
Frame_DataEditor.grid(row=0,column=1, padx=16, sticky=N)
root.grid_columnconfigure(1, minsize=200)

Frame_OptionHolder = Frame(Frame_DataEditor)

Var_Mode = StringVar()
Var_Mode.set(ModeList[0])
OptionMenu_SelectMode = OptionMenu(Frame_OptionHolder,Var_Mode,*ModeList, command=ChangeMode).grid(row=0,column=0)

Frame_ChangeNumber = Frame(Frame_OptionHolder)
Var_Number = IntVar()
Button_Decrease = Button(Frame_ChangeNumber,command=Decrease,text="-").grid(row=0,column=1)
Label_Number = Label(Frame_ChangeNumber,textvariable=Var_Number).grid(row=0,column=2)
Button_Decrease = Button(Frame_ChangeNumber,command=Increase,text="+").grid(row=0,column=3)

Frame_ChangeType = Frame(Frame_OptionHolder)
Var_Type = StringVar()
Var_Type.set(TypeList[0])
OptionMenu_ChangeType = OptionMenu(Frame_ChangeType,Var_Type,*TypeList, command=ChangeMode).grid(row=0,column=1)

Frame_OptionHolder.grid(row=0,column=0)

#------------
#Data Editing
#------------

GrassVars = []
for i in range(0,15):
    GrassVars.append(IntVar())
ExitVars = []
for i in range(0,3):
    ExitVars.append(IntVar())
DoorVars = []
for i in range(0,5):
    DoorVars.append(IntVar())
TrainerVars = []
for i in range(0,17):
    TrainerVars.append(IntVar())

def LoadVars():
    global zonedata
    for i in range(0,15):
        GrassVars[i].set(zonedata[((0+i)*16) + Var_Number.get()])
    for i in range(0,3):
        ExitVars[i].set(zonedata[((15+i)*16) + Var_Number.get()])
    for i in range(0,5):
        DoorVars[i].set(zonedata[((18+i)*16) + Var_Number.get()])
    for i in range(0,17):
        TrainerVars[i].set(zonedata[((23+i)*16) + Var_Number.get()])

def SaveVars():
    global zonedata
    for i in range(0,15):
        zonedata[((0+i)*16) + Var_Number.get()] = GrassVars[i].get()
    for i in range(0,3):
        zonedata[((15+i)*16) + Var_Number.get()] = ExitVars[i].get()
    for i in range(0,5):
        zonedata[((18+i)*16) + Var_Number.get()] = DoorVars[i].get()
    for i in range(0,17):
        zonedata[((23+i)*16) + Var_Number.get()] = TrainerVars[i].get()

def SwitchNumber(newNumber):
    SaveVars()
    Var_Number.set(newNumber)
    LoadVars()

LoadVars()

DataFrames = []
for i in range(0,4):
    DataFrames.append(Frame(Frame_DataEditor))
    

Label(DataFrames[0],text="Wild Pokemon Ids").grid(row=0,column=0)
Entry(DataFrames[0],textvariable=GrassVars[0]).grid(row=1,column=0)
Entry(DataFrames[0],textvariable=GrassVars[1]).grid(row=2,column=0)
Entry(DataFrames[0],textvariable=GrassVars[2]).grid(row=3,column=0)
Entry(DataFrames[0],textvariable=GrassVars[3]).grid(row=4,column=0)
Entry(DataFrames[0],textvariable=GrassVars[4]).grid(row=5,column=0)
Label(DataFrames[0],text="Minimum Levels").grid(row=0,column=1)
Entry(DataFrames[0],textvariable=GrassVars[5]).grid(row=1,column=1)
Entry(DataFrames[0],textvariable=GrassVars[6]).grid(row=2,column=1)
Entry(DataFrames[0],textvariable=GrassVars[7]).grid(row=3,column=1)
Entry(DataFrames[0],textvariable=GrassVars[8]).grid(row=4,column=1)
Entry(DataFrames[0],textvariable=GrassVars[9]).grid(row=5,column=1)
Label(DataFrames[0],text="Maximum Levels").grid(row=0,column=2)
Entry(DataFrames[0],textvariable=GrassVars[10]).grid(row=1,column=2)
Entry(DataFrames[0],textvariable=GrassVars[11]).grid(row=2,column=2)
Entry(DataFrames[0],textvariable=GrassVars[12]).grid(row=3,column=2)
Entry(DataFrames[0],textvariable=GrassVars[13]).grid(row=4,column=2)
Entry(DataFrames[0],textvariable=GrassVars[14]).grid(row=5,column=2)

Label(DataFrames[1],text="New Area").grid(row=0,column=0)
Entry(DataFrames[1],textvariable=ExitVars[0]).grid(row=1,column=0)
Label(DataFrames[1],text="X Pos").grid(row=0,column=1)
Entry(DataFrames[1],textvariable=ExitVars[1]).grid(row=1,column=1)
Label(DataFrames[1],text="Y Pos").grid(row=0,column=2)
Entry(DataFrames[1],textvariable=ExitVars[2]).grid(row=1,column=2)

Label(DataFrames[2],text="New Building").grid(row=0,column=0)
Entry(DataFrames[2],textvariable=DoorVars[0]).grid(row=1,column=0)
Label(DataFrames[2],text="X Pos").grid(row=0,column=1)
Entry(DataFrames[2],textvariable=DoorVars[1]).grid(row=1,column=1)
Label(DataFrames[2],text="Y Pos").grid(row=0,column=2)
Entry(DataFrames[2],textvariable=DoorVars[2]).grid(row=1,column=2)
Label(DataFrames[2],text="NPC 1").grid(row=0,column=3)
Entry(DataFrames[2],textvariable=DoorVars[3]).grid(row=1,column=3)
Label(DataFrames[2],text="NPC 2").grid(row=0,column=4)
Entry(DataFrames[2],textvariable=DoorVars[4]).grid(row=1,column=4)

Label(DataFrames[3],text="Trainer Pokemon Ids").grid(row=0,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[0]).grid(row=1,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[1]).grid(row=2,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[2]).grid(row=3,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[3]).grid(row=4,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[4]).grid(row=5,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[5]).grid(row=6,column=0)
Label(DataFrames[3],text="Levels").grid(row=0,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[6]).grid(row=1,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[7]).grid(row=2,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[8]).grid(row=3,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[9]).grid(row=4,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[10]).grid(row=5,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[11]).grid(row=6,column=1)
Label(DataFrames[3],text="X Pos").grid(row=7,column=0)
Entry(DataFrames[3],textvariable=TrainerVars[12]).grid(row=8,column=0)
Label(DataFrames[3],text="Y Pos").grid(row=7,column=1)
Entry(DataFrames[3],textvariable=TrainerVars[13]).grid(row=8,column=1)
Label(DataFrames[3],text="Direction(1=R 2=L 3=D 4=U)").grid(row=9,column=0,columnspan=2)
Entry(DataFrames[3],textvariable=TrainerVars[14]).grid(row=10,column=0,columnspan=2)
Label(DataFrames[3],text="Text ID").grid(row=11,column=0,columnspan=2)
Entry(DataFrames[3],textvariable=TrainerVars[15]).grid(row=12,column=0,columnspan=2)
Label(DataFrames[3],text="Reward").grid(row=13,column=0,columnspan=2)
Entry(DataFrames[3],textvariable=TrainerVars[16]).grid(row=14,column=0,columnspan=2)

#-----
#Tools
#-----

Pokemon_Names = "None,Bulbasaur,Ivysaur,Venusaur,Charmander,Charmeleon,Charizard,Squirtle,Wartortle,Blastoise,Caterpie,Metapod,Butterfree,Weedle,Kakuna,Beedrill,Pidgey,Pidgeotto,Pidgeot,Rattata,Raticate,Spearow,Fearow,Ekans,Arbok,Pikachu,Raichu,Sandshrew,Sandslash,Nidoran(F),Nidorina,Nidoqueen,Nidoran(M),Nidorino,Nidoking,Clefairy,Clefable,Vulpix,Ninetales,Jigglypuff,Wigglytuff,Zubat,Golbat,Oddish,Gloom,Vileplume,Paras,Parasect,Venonat,Venomoth,Diglett,Dugtrio,Meowth,Persian,Psyduck,Golduck,Mankey,Primeape,Growlithe,Arcanine,Poliwag,Poliwhirl,Poliwrath,Abra,Kadabra,Alakazam,Machop,Machoke,Machamp,Bellsprout,Weepinbell,Victreebel,Tentacool,Tentacruel,Geodude,Graveler,Golem,Ponyta,Rapidash,Slowpoke,Slowbro,Magnemite,Magneton,Farfetch'd,Doduo,Dodrio,Seel,Dewgong,Grimer,Muk,Shellder,Cloyster,Gastly,Haunter,Gengar,Onix,Drowzee,Hypno,Krabby,Kingler,Voltorb,Electrode,Exeggcute,Exeggutor,Cubone,Marowak,Hitmonlee,Hitmonchan,Lickitung,Koffing,Weezing,Rhyhorn,Rhydon,Chansey,Tangela,Kangaskhan,Horsea,Seadra,Goldeen,Seaking,Staryu,Starmie,Mr. Mime,Scyther,Jynx,Electabuzz,Magmar,Pinsir,Tauros,Magikarp,Gyarados,Lapras,Ditto,Eevee,Vaporeon,Jolteon,Flareon,Porygon,Omanyte,Omastar,Kabuto,Kabutops,Aerodactyl,Snorlax,Articuno,Zapdos,Moltres,Dratini,Dragonair,Dragonite,Mewtwo,Mew,"
Pokemon_Names = Pokemon_Names.split(',')
Item_Names = 'Absorb,Acid,Acid Armor,Agility,Amnesia,Aurora Beam,Barrage,Barrier,Bide,Bind,Bite,Blizzard,Body Slam,Bone Club,Bonemerang,Bubble,Bubble Beam,Clamp,Comet Punch,Confuse Ray,Confusion,Constrict,Conversion,Counter,Crabhammer,Cut,Defense Curl,Dig,Disable,Dizzy Punch,Double Kick,Double Slap,Double Team,Double-Edge,Dragon Rage,Dream Eater,Drill Peck,Earthquake,Egg Bomb,Ember,Explosion,Fire Blast,Fire Punch,Fire Spin,Fissure,Flamethrower,Flash,Fly,Focus Energy,Fury Attack,Fury Swipes,Glare,Growl,Growth,Guillotine,Gust,Harden,Haze,Headbutt,High Jump Kick,Horn Attack,Horn Drill,Hydro Pump,Hyper Beam,Hyper Fang,Hypnosis,Ice Beam,Ice Punch,Jump Kick,Karate Chop,Kinesis,Leech Life,Leech Seed,Leer,Lick,Light Screen,Lovely Kiss,Low Kick,Meditate,Mega Drain,Mega Kick,Mega Punch,Metronome,Mimic,Minimize,Mirror Move,Mist,Night Shade,Pay Day,Peck,Petal Dance,Pin Missile,Poison Gas,Poison Powder,Poison Sting,Pound,Psybeam,Psychic,Psywave,Quick Attack,Rage,Razor Leaf,Razor Wind,Recover,Reflect,Rest,Roar,Rock Slide,Rock Throw,Rolling Kick,Sand Attack,Scratch,Screech,Seismic Toss,Self-Destruct,Sharpen,Sing,Skull Bash,Sky Attack,Slam,Slash,Sleep Powder,Sludge,Smog,Smokescreen,Soft-Boiled,Solar Beam,Sonic Boom,Spike Cannon,Splash,Spore,Stomp,Strength,String Shot,Struggle,Stun Spore,Submission,Substitute,Super Fang,Supersonic,Surf,Swift,Swords Dance,Tackle,Tail Whip,Take Down,Teleport,Thrash,Thunder,Thunder Punch,Thunder Shock,Thunder Wave,Thunderbolt,Toxic,Transform,Tri Attack,Twineedle,Vice Grip,Vine Whip,Water Gun,Waterfall,Whirlwind,Wing Attack,Withdraw,Wrap'
Item_Names = ["","Pokeball","Great Ball","Ultra Ball","Master Ball","Potion","Super Potion","Hyper Potion","Max Potion","Status Heal","Full Restore","Antidote","Burn Heal","Parlyz Heal","Ice Heal","Awakening","Thunder Stone","Moon Stone","Fire Stone","Leaf Stone","Water Stone"] + Item_Names.split(',')

def IsInt(s):
    try: 
        int(s)
        return True
    except ValueError:
        return False


def pokemonid():
    if(IsInt(Var_PokemonToConvert.get())):
        Var_PokemonToConvert.set(Pokemon_Names[int(Var_PokemonToConvert.get())])
    else:
        for x in range(0,len(Pokemon_Names)):
            if(Pokemon_Names[x].lower().replace(' ','') == Var_PokemonToConvert.get().lower().replace(' ','')):
                Var_PokemonToConvert.set(str(x))

def itemid():
    if(IsInt(Var_ItemToConvert.get())):
        Var_ItemToConvert.set(Item_Names[int(Var_ItemToConvert.get())])
    else:
        for x in range(0,len(Item_Names)):
            if(Item_Names[x].lower().replace(' ','') == Var_ItemToConvert.get().lower().replace(' ','')):
                Var_ItemToConvert.set(str(x))

Var_PokemonToConvert = StringVar()
Var_ItemToConvert = StringVar()

Canvas_SetMap.grid_rowconfigure(15, minsize=20)
Entry(Canvas_SetMap,textvar=Var_PokemonToConvert).grid(row=16,column=0)
Button(Canvas_SetMap,text="Convert Pokemon to ID",command=pokemonid).grid(row=17,column=0)
Entry(Canvas_SetMap,textvar=Var_ItemToConvert).grid(row=18,column=0)
Button(Canvas_SetMap,text="Convert Item to ID",command=itemid).grid(row=19,column=0)

#Trainer:

Canvas_SetMap.grid_rowconfigure(20, minsize=20)
def DecreaseTrainer():
    if(currentTrainer.get()-1==-1):
        UpdateTrainer(trainerTextCount-1)
    else:
        UpdateTrainer(currentTrainer.get()-1)
def IncreaseTrainer():
    if(currentTrainer.get()+1==trainerTextCount):
        UpdateTrainer(0)
    else:
        UpdateTrainer(currentTrainer.get()+1)

Label(Canvas_SetMap,text="Trainer Text").grid(row=21,column=0)
Frame_ChangeTrainer = Frame(Canvas_SetMap)
Button(Frame_ChangeTrainer,command=DecreaseTrainer,text="-").grid(row=0,column=0)
Label(Frame_ChangeTrainer,textvariable=currentTrainer).grid(row=0,column=1)
Button(Frame_ChangeTrainer,command=IncreaseTrainer,text="+").grid(row=0,column=2)
Frame_ChangeTrainer.grid(row=22,column=0)

Entry(Canvas_SetMap,textvar=Var_TrainerText).grid(row=23,column=0)

#Trainer:

Canvas_SetMap.grid_rowconfigure(24, minsize=20)
def DecreaseNpc():
    if(currentNpc.get()-1==-1):
        UpdateNpc(npcCount-1)
    else:
        UpdateNpc(currentNpc.get()-1)
def IncreaseNpc():
    if(currentNpc.get()+1==npcCount):
        UpdateNpc(0)
    else:
        UpdateNpc(currentNpc.get()+1)

Label(Canvas_SetMap,text="Npc Text/Reward").grid(row=25,column=0)
Frame_ChangeNpc = Frame(Canvas_SetMap)
Button(Frame_ChangeNpc,command=DecreaseNpc,text="-").grid(row=0,column=0)
Label(Frame_ChangeNpc,textvariable=currentNpc).grid(row=0,column=1)
Button(Frame_ChangeNpc,command=IncreaseNpc,text="+").grid(row=0,column=2)
Frame_ChangeNpc.grid(row=26,column=0)

Entry(Canvas_SetMap,textvar=Var_NpcText).grid(row=27,column=0)
Entry(Canvas_SetMap,textvar=Var_NpcReward).grid(row=28,column=0)

#----
#Loop
#----


running = True
def disable_event():
    global running
    running = False
root.protocol("WM_DELETE_WINDOW", disable_event)


while running:
    draw()
    root.update()

UpdateTrainer(currentTrainer.get())#saves
UpdateNpc(currentNpc.get())#saves
pygame.quit()
root.destroy()
sys.exit()
