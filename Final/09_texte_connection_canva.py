import ctypes
import tkinter as tk
import os
from tkinter import messagebox

# Define C structures in Python
class Force(ctypes.Structure):
    _fields_ = [("fx", ctypes.c_double),
                ("fy", ctypes.c_double)]

class Sheep(ctypes.Structure):
    _fields_ = [("x", ctypes.c_double),
                ("y", ctypes.c_double),
                ("r", ctypes.c_double),
                ("force", Force),
                ("inRoom", ctypes.c_bool)]

class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_double),
                ("y", ctypes.c_double)]

class Line(ctypes.Structure):
    _fields_ = [("start", Point),
                ("end", Point)]

class Room(ctypes.Structure):
    _fields_ = [("exitRoom", Line),
                ("sizeRoom", Point)]

# Load the shared library
#my_library = ctypes.CDLL(r"C:\Users\victo\OneDrive - epfl.ch\BA3\cmt\PROJET\tests persos\09_finalTom.so")  # Replace './09_finalTom.so' with the actual path to your shared library
my_library = ctypes.CDLL(os.path.join(os.path.dirname(__file__), '01_templateC.so'))

# Set up Tkinter window
window = tk.Tk()
window.title("Sheep Movement Simulation")

# Canvas dimensions
canvas_width = 500
canvas_height = 500

# Create canvas
canvas = tk.Canvas(window, width=canvas_width, height=canvas_height, bg="white")
canvas.pack()

# Seed random generator function
my_library.seedRandomGenerator.argtypes = [ctypes.c_uint]

# Function to generate the room
my_library.generateRoom.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]

# Function to generate random sheeps
my_library.generateRandomSheeps.argtypes = [ctypes.c_int, ctypes.c_double]
my_library.generateRandomSheeps.restype = ctypes.POINTER(Sheep)

# Function to check if any sheep is still inside the room
my_library.sheepStillInside.argtypes = [ctypes.c_int]
my_library.sheepStillInside.restype = ctypes.c_int

# Function to move sheeps and return their coordinates
my_library.moveSheep.argtypes = [ctypes.c_int]
my_library.moveSheep.restype = ctypes.POINTER(Point)

# Function to free matrix memory
my_library.freeMatrix.argtypes = [ctypes.POINTER(Sheep)]

############## PROTOTYPE 3 : marche, crée un timer et un bouton non clickable 2 fois
import time

# Global variables for simulation control
simulation_speed = 10  # Adjust this value to control the simulation speed (milliseconds)
nbSheep = 200
seed = int(time.time())
sheep_array = None  # To store the array of sheep
start_time = None  # To store the start time

# Example usage
def start_simulation():
    global sheep_array, start_time

    # Disable the button after clicking
    start_button.config(state=tk.DISABLED)

    # Record the start time
    start_time = time.time()

    # Seed random generator
    my_library.seedRandomGenerator(seed)

    # Generate room and random sheeps
    room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y = 400.0, 400.0, 400, 180, 400, 220
    my_library.generateRoom(room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y)
    sheep_array = my_library.generateRandomSheeps(nbSheep, 5.0)

    # Start the simulation loop
    simulate_movement()

def simulate_movement():
    global sheep_array, start_time
    # Call moveSheep function
    result = my_library.moveSheep(nbSheep)

    # Clear previous sheep drawings
    canvas.delete("sheep")

    canvas.create_rectangle(50, 50, 50 + 400, 50 + 400, outline="black", width=4)
    canvas.create_line(450, 230, 450, 270, fill="red", width=4)

    # Draw new sheep positions
    for i in range(nbSheep):
        x, y = result[i].x + 50, result[i].y + 50
        canvas.create_oval(x - 5, y - 5, x + 5, y + 5, fill="blue", tags="sheep")

    # Update the canvas
    window.update()

    # Free allocated memory using the correct type
    my_library.freeMatrix(ctypes.cast(result, ctypes.POINTER(Sheep)))

    # Check if any sheep is still inside
    if my_library.sheepStillInside(nbSheep):
        # Calculate elapsed time
        elapsed_time = time.time() - start_time
        elapsed_time_str = f"Elapsed Time: {int(elapsed_time)} seconds"
        chronometer_label.config(text=elapsed_time_str)

        # Schedule the next simulation step
        window.after(simulation_speed, simulate_movement)
    else:
        # Display a message when all sheep have exited
        messagebox.showinfo("Simulation Complete", "All sheep have exited the room.")


# Button to start the simulation
start_button = tk.Button(window, text="Start Simulation", command=start_simulation)
start_button.pack()

# Label for chronometer
chronometer_label = tk.Label(window, text="Elapsed Time: 0 seconds")
chronometer_label.pack()

# Keep the window open after all sheep exit
window.mainloop()