import ctypes
import time
import tkinter as tk
from unittest import result

seed_value = int(time.time())

# Define the Force strut in Python
class Force(ctypes.Structure):
    _fields_ = [("fx", ctypes.c_double),
                ("fy", ctypes.c_double)]

# Define the Sheep struct in Python
class Sheep(ctypes.Structure):
    _fields_ = [("x", ctypes.c_double),
                ("y", ctypes.c_double),
                ("r", ctypes.c_double),
                ("force", Force),
                ("inRoom", ctypes.c_bool)]


# Load the shared library
lib = ctypes.CDLL(r"c:\Users\tomje\OneDrive\EPFL\BA3\CMT\Pro-cul-feur\libsheep.so")

# Define the function prototypes
lib.moveSheep.argtypes = [ctypes.POINTER(Sheep), ctypes.c_int]
lib.moveSheep.restype = ctypes.POINTER(Sheep)

# Define the function prototype for generateRandomSheeps
lib.generateRandomSheeps.argtypes = [ctypes.POINTER(Sheep), ctypes.c_int, ctypes.c_double]
lib.generateRandomSheeps.restype = None

# Define the function prototype for generateRoom
lib.generateRoom.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
lib.generateRoom.restype = None

# Define the function prototype for freeMatrix
lib.freeMatrix.argtypes = [ctypes.POINTER(Sheep)]
lib.freeMatrix.restype = None

# Define the function prototype for seedRandomGenerator
lib.seedRandomGenerator.argtypes = [ctypes.c_uint]
lib.seedRandomGenerator.restype = None

# Define the function prototype for sheepStillInside
lib.sheepStillInside.argtypes = [ctypes.POINTER(Sheep), ctypes.c_int]
lib.sheepStillInside.restype = ctypes.c_int

# Define the function prototype for updateForces
lib.updateForces.argtypes = [ctypes.POINTER(Sheep), ctypes.c_int]
lib.updateForces.restype = None


# Create a tkinter window
root = tk.Tk()
canvas = tk.Canvas(root, width=500, height=500)
canvas.pack()

# Create some sheep
nbSheep = 10
sheepArray = (Sheep * nbSheep)()

# Call the function to initialize the sheep
lib.seedRandomGenerator(seed_value)
lib.generateRoom(500.0, 500.0, 500.0, 240.0, 500.0, 260.0)
lib.generateRandomSheeps(sheepArray, nbSheep, 10.0)

# Draw the sheep
sheep_ids = [canvas.create_oval(sheep.x - 5, sheep.y - 5, sheep.x + 5, sheep.y + 5) for sheep in sheepArray]

root.mainloop()

lib.freeMatrix(sheepArray)

def update_positions():
    # Move the sheep
    while(lib.sheepStillInside(sheepArray, nbSheep)):
        lib.updateForces(sheepArray, nbSheep)
        sheepArray = lib.moveSheep(sheepArray, nbSheep)
        sheep_ids = [canvas.create_oval(sheep.x - 5, sheep.y - 5, sheep.x + 5, sheep.y + 5) for sheep in sheepArray]
    

# Start the animation
""" update_positions() """
#root.mainloop()
