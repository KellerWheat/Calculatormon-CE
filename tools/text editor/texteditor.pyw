from tkinter import *
import numpy as np
import os
import sys
import pyperclip

root = Tk()
root.title('Map Editor')
root.wm_state('zoomed')

textCount = 255

stringIndex = 0
textTitles = []
textContent = []
textFrames = []

currentTitleVar = np.array([],dtype=object)
currentContentVar = np.array([],dtype=object)



def ExportData():
    global textTitles, textContent, textCount, output, outputstring
    outputstring = ""

    for x in range(0,textCount):
        outputstring += textContent[x].get('1.0', 'end-1c').ljust(256, '\0')
    output = np.array([ord(c) for c in outputstring], 'B')
    output.tofile('PKMNTD.bin')
    os.startfile("converttext.bat")

def Load():
    global textTitles, textContent
    tempArray = np.array(['']*256,dtype=object)
    if(os.path.isfile("textTitles.npy")):
        tempArray = np.load("textTitles.npy")
        for x in range(0,textCount):
            textTitles.append(Text(textFrames[x], width=32, height=1))
            textTitles[x].insert("1.0",tempArray[x])
    else:
        for x in range(0,textCount):
            textTitles.append(Text(textFrames[x], width=32, height=1)) 
        
    if(os.path.isfile("textContent.npy")):
        tempArray = np.load("textContent.npy")
        for x in range(0,textCount):
            textContent.append(Text(textFrames[x], width=32, height=8))
            textContent[x].insert("1.0",tempArray[x])
    else:
        for x in range(0,textCount):
            textContent.append(Text(textFrames[x], width=32, height=8))
    
            

def Save():
    global textTitles, textContent
    tempArray = np.array(['']*256,dtype=object)
    for x in range(0,textCount):
        tempArray[x] = textTitles[x].get('1.0', 'end-1c')
    np.save("textTitles.npy",tempArray)
    tempArray = np.array(['']*256,dtype=object)
    for x in range(0,textCount):
        tempArray[x] = textContent[x].get('1.0', 'end-1c')
    np.save("textContent.npy",tempArray)

class Scrollable(Frame):
    """
       Make a frame scrollable with scrollbar on the right.
       After adding or removing widgets to the scrollable frame, 
       call the update() method to refresh the scrollable area.
    """

    def __init__(self, frame, width=16):

        scrollbar = Scrollbar(frame, width=width)
        scrollbar.pack(side=RIGHT, fill=Y, expand=False)

        self.canvas = Canvas(frame, yscrollcommand=scrollbar.set)
        self.canvas.pack(side=LEFT, fill=BOTH, expand=True)

        scrollbar.config(command=self.canvas.yview)

        self.canvas.bind('<Configure>', self.__fill_canvas)

        # base class initialization
        Frame.__init__(self, frame)         

        # assign this obj (the inner frame) to the windows item of the canvas
        self.windows_item = self.canvas.create_window(0,0, window=self, anchor=NW)


    def __fill_canvas(self, event):
        "Enlarge the windows item to the canvas width"

        canvas_width = event.width
        self.canvas.itemconfig(self.windows_item, width = canvas_width)        

    def update(self):
        "Update the canvas and the scrollregion"

        self.update_idletasks()
        self.canvas.config(scrollregion=self.canvas.bbox(self.windows_item))

frame = Scrollable(root, width=32)

for x in range(0,textCount):
    textFrames.append(Frame(frame))
Load()
for x in range(0,textCount):
    Label(textFrames[x],text=str(x) + " - ").grid(row = 0, column=0)
    textTitles[x].grid(row = 0, column=1)
    textContent[x].grid(row = 1, column=0, columnspan = 2, sticky=E)
    textFrames[x].grid(row=1+int(x/6), column=int(x%6), padx=10, pady=10)
    textFrames[x].update()

Button(frame,command=Save, text="Save Data").grid(row=0,column=0, stick=N)
Button(frame,command=ExportData, text="Export Data").grid(row=0,column=1, stick=N)

frame.update()


root.mainloop()
