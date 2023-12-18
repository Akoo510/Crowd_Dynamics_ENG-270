import ctypes
import tkinter as tk

# Load the C library
lib = ctypes.CDLL(r"C:\Users\tomje\OneDrive\EPFL\BA3\CMT\Pro-cul-feur\01_templateC.so")

# Define the Sheep struct
class Sheep(ctypes.Structure):
    _fields_ = [("x", ctypes.c_double),
                ("y", ctypes.c_double),
                ("r", ctypes.c_double),
                ("force", ctypes.c_double * 2),
                ("inRoom", ctypes.c_bool)]

# Define the Room struct
class Room(ctypes.Structure):
    _fields_ = [("sizeRoom", ctypes.c_double * 2),
                ("start", ctypes.c_double * 2),
                ("end", ctypes.c_double * 2)]

# Initialize the room and sheep
lib.generateRoom(400, 400, 400, 190, 400, 210)
nbSheep = 400
sheepolata = (Sheep * nbSheep)()
lib.generateRandomSheeps(sheepolata, nbSheep, 1.0)

# Create a Tkinter window
root = tk.Tk()
canvas = tk.Canvas(root, width=400, height=400)
canvas.pack()

# Draw the initial positions of the sheep
for i in range(nbSheep):
    x = sheepolata[i].x
    y = sheepolata[i].y
    r = sheepolata[i].r
    canvas.create_oval(x-r, y-r, x+r, y+r, fill='black')

# Update the positions of the sheep
def update_positions():
    lib.updateForces(sheepolata, nbSheep)
    lib.moveSheep(sheepolata, nbSheep)
    for i in range(nbSheep):
        x = sheepolata[i].x
        y = sheepolata[i].y
        r = sheepolata[i].r
        canvas.create_oval(x-r, y-r, x+r, y+r, fill='black')
    root.after(100, update_positions)  # Update every 100 ms

# Start the loop
root.after(100, update_positions)
root.mainloop()