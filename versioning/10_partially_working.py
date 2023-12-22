# Pour la GUI
import tkinter as tk
from tkinter import ttk, messagebox
from math import pi
from tkinter import Toplevel

# Pour la shared library :
import subprocess
import ctypes

import os
import time 

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
    
my_library = ctypes.CDLL(os.path.join(os.path.dirname(__file__), '10_theCode.so'))
#my_library = ctypes.CDLL(os.path.join(os.path.dirname(__file__), '09_finalTom.so'))

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


class DataEntryForm:
    def __init__(self, root):
        self.root = root
        self.root.title("Data Entry Form")

        self.individual_speed = None
        self.num_individuals = None

        self.step = 1  # Initial step

        # Initialize variables to store user inputs
        self.individual_vars = {
            'num_individuals': tk.StringVar(),
            'individual_type': tk.StringVar(),
            'individual_speed': tk.StringVar()
        }

        self.room_vars = {
            'x_room': tk.StringVar(),
            'y_room': tk.StringVar(),
            'exit_type': tk.StringVar(),
            'wall': tk.StringVar(),
        }

        self.exit_coords_vars = {
            'exit_coords': tk.StringVar(),
        }

        # Dictionary for individual radius
        self.radius_dict = {
            "Human": 0.3,
            "Cow": 0.5,
            "Sheep": 0.15,
            "Ant": 0.002,
            "Elephant": 1.5,
        }

        # Dictionary for exit dimensions
        self.exit_dict = {
            "Standard door": 0.8,
            "Double door": 1.6,
            "Ant nest hole": 0.01,
            "Paddock gate": 5,
        }

        self.create_widgets()

    def create_widgets(self):
        if self.step == 1:
            self.create_step1_widgets()
        elif self.step == 2:
            self.create_step2_widgets()
        elif self.step == 3:
            self.create_step3_widgets()

    def create_step1_widgets(self):
        frame = tk.Frame(self.root)
        frame.pack(padx=10, pady=10)

        nb_individuals_label = tk.Label(frame, text="Number of individuals wanted")
        nb_individuals_entry = tk.Entry(frame, textvariable=self.individual_vars['num_individuals'])
        nb_individuals_label.grid(row=0, column=0, padx=5, pady=5)
        nb_individuals_entry.grid(row=0, column=1, padx=5, pady=5)

        type_individuals_label = tk.Label(frame, text="Individual type wanted")
        type_individuals_combobox = ttk.Combobox(frame, values=['Human', 'Cow', 'Ant', 'Sheep', 'Elephant'],
                                                textvariable=self.individual_vars['individual_type'], state='readonly')
        type_individuals_label.grid(row=1, column=0, padx=5, pady=5)
        type_individuals_combobox.grid(row=1, column=1, padx=5, pady=5)

        speed_individuals_label = tk.Label(frame, text="Speed of the individuals [ms]")
        speed_individuals_entry = tk.Entry(frame, textvariable=self.individual_vars['individual_speed'])
        speed_individuals_label.grid(row=2, column=0, padx=5, pady=5)
        speed_individuals_entry.grid(row=2, column=1, padx=5, pady=5)

        validate_button = tk.Button(frame, text="Validate", command=self.validate_step1)
        validate_button.grid(row=3, column=0, columnspan=2, pady=10)

    def create_step2_widgets(self):
        frame = tk.Frame(self.root)
        frame.pack(padx=10, pady=10)

        x_room_label = tk.Label(frame, text="Choose the length of the room")
        x_room_entry = tk.Entry(frame, textvariable=self.room_vars['x_room'])
        x_room_label.grid(row=0, column=0, padx=5, pady=5)
        x_room_entry.grid(row=0, column=1, padx=5, pady=5)

        y_room_label = tk.Label(frame, text="Choose the height of the room")
        y_room_entry = tk.Entry(frame, textvariable=self.room_vars['y_room'])
        y_room_label.grid(row=1, column=0, padx=5, pady=5)
        y_room_entry.grid(row=1, column=1, padx=5, pady=5)

        exit_type_label = tk.Label(frame, text="Exit type wanted")
        exit_type_combobox = ttk.Combobox(frame, values=['Standard door', 'Double door', 'Ant nest hole', 'Paddock gate'],
                                          textvariable=self.room_vars['exit_type'], state='readonly')
        exit_type_label.grid(row=2, column=0, padx=5, pady=5)
        exit_type_combobox.grid(row=2, column=1, padx=5, pady=5)

        wall_label = tk.Label(frame, text="Wall wanted for the exit")
        wall_combobox = ttk.Combobox(frame, values=['Right', 'Left', 'Top', 'Bottom'], textvariable=self.room_vars['wall'], state='readonly')
        wall_label.grid(row=3, column=0, padx=5, pady=5)
        wall_combobox.grid(row=3, column=1, padx=5, pady=5)

        validate_button = tk.Button(frame, text="Validate", command=self.validate_step2)
        validate_button.grid(row=4, column=0, columnspan=2, pady=10)

    def create_step3_widgets(self):
        frame = tk.Frame(self.root)
        frame.pack(padx=10, pady=10)

        exit_coords_label = tk.Label(frame, text="Choose the length at which you want the exit to be on the wall")
        exit_coords_entry = tk.Entry(frame, textvariable=self.exit_coords_vars['exit_coords'])
        exit_coords_label.grid(row=0, column=0, padx=5, pady=5)
        exit_coords_entry.grid(row=0, column=1, padx=5, pady=5)

        validate_button = tk.Button(frame, text="Validate", command=self.validate_step3)
        validate_button.grid(row=1, column=0, columnspan=2, pady=10)

    def validate_step1(self):
        # Validate step 1 inputs
        num_individuals = self.individual_vars['num_individuals'].get()
        individual_type = self.individual_vars['individual_type'].get()
        individual_speed = self.individual_vars['individual_speed'].get()

        if not num_individuals.isdigit() or int(num_individuals) == 0 or not individual_type or not individual_speed.isdigit() or int(individual_speed) == 0:
            messagebox.showwarning(title="Error", message="Invalid input. Please enter valid, positive and non-zero values.")
        else:
            # If validation is successful, move to the next step, disable step 1 widgets, and create step 2 widgets
            self.disable_step1_widgets()
            self.step += 1
            self.create_widgets()

    def validate_step2(self):
        # Validate step 2 inputs
        x_room_str = self.room_vars['x_room'].get()
        y_room_str = self.room_vars['y_room'].get()
        exit_type = self.room_vars['exit_type'].get()
        wall = self.room_vars['wall'].get()
        individual_type = self.individual_vars['individual_type'].get()
        num_individuals_str = self.individual_vars['num_individuals'].get()
        individual_speed_str = self.individual_vars['individual_speed'].get()

        try:
            x_room = float(x_room_str)
            y_room = float(y_room_str)
            num_individuals = int(num_individuals_str)
            individual_speed = float(individual_speed_str)
        except ValueError:
            messagebox.showwarning(title="Error", message="Invalid input. Please enter valid numeric values for room dimensions and number of individuals.")
            return

        if not x_room > 0 or not y_room > 0 or not exit_type or not wall or not num_individuals > 0:
            messagebox.showwarning(title="Error", message="Invalid input. Please enter valid values.")
        elif self.radius_dict[individual_type]*2 > self.exit_dict[exit_type]:
            messagebox.showwarning(title="Error", message=f"Chosen exit type must be greater than or equal to the diameter of the {individual_type}.")
        else:
            # Calculate the total area required for the individuals
            total_area_required = num_individuals * (pi * self.radius_dict[individual_type] ** 2)

            # Check if the room area is sufficient
            if x_room * y_room < total_area_required:
                messagebox.showwarning(title="Error", message="Room dimensions are too small to accommodate all individuals.")
            else:
            # Store additional attributes for later use
                self.individual_speed = float(individual_speed)
                self.num_individuals = int(num_individuals)

                # If all conditions are met, move to the next step, disable step 2 widgets, and create step 3 widgets
                self.disable_step2_widgets()
                self.step += 1
                self.create_widgets()

    def validate_step3(self):
        # Validate step 3 inputs
        exit_coords_str = self.exit_coords_vars['exit_coords'].get()

        try:
            exit_coords = float(exit_coords_str)
        except ValueError:
            messagebox.showwarning(title="Error", message="Invalid input. Please enter a valid numeric value for exit coordinates.")
            return

        # Validate additional condition for exit coordinates
        x_room = float(self.room_vars['x_room'].get())
        y_room = float(self.room_vars['y_room'].get())
        wall = self.room_vars['wall'].get()

        exit_half_length = self.exit_dict[self.room_vars['exit_type'].get()] / 2

        if wall == "Left" or wall == "Right":
            if not 0 <= exit_coords <= y_room:
                messagebox.showwarning(title="Warning", message="Exit coordinates must be between 0 and room height.")
                return
            if not 0 + exit_half_length <= exit_coords <= y_room - exit_half_length:
                messagebox.showwarning(title="Warning", message=f"The chosen exit type exceeds the size of the {wall} wall at this position. It must be at least at +/- {exit_half_length}m from the edge of the wall.")
                return
        elif wall == "Top" or wall == "Bottom":
            if not 0 <= exit_coords <= x_room:
                messagebox.showwarning(title="Warning", message="Exit coordinates must be between 0 and room length.")
                return
            if not 0 + exit_half_length <= exit_coords <= x_room - exit_half_length:
                messagebox.showwarning(title="Warning", message=f"The chosen exit type exceeds the size of the {wall} at this position. It must be at least {exit_half_length}m from the edge of the wall.")
                return

        # If all conditions are met, perform the simulation and show an info message
        self.perform_simulation()
        # Disable step 3 widgets after successful validation
        self.disable_step3_widgets()
        # Show info message after disabling step 3 widgets
        messagebox.showinfo(title="Simulation", message="Simulation completed successfully!")

    def disable_step1_widgets(self):
        # Disable step 1 widgets
        for widget in self.root.winfo_children():
            if isinstance(widget, tk.Frame):
                for child_widget in widget.winfo_children():
                    if isinstance(child_widget, tk.Entry) and child_widget != self.individual_vars['num_individuals']:
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, ttk.Combobox) and child_widget != self.individual_vars['individual_type']:
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, ttk.Combobox) and child_widget != self.individual_vars['individual_speed']:
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, tk.Button) and child_widget['text'] == 'Validate':
                        child_widget.config(state='disabled')

    def disable_step2_widgets(self):
        # Disable step 2 widgets
        for widget in self.root.winfo_children():
            if isinstance(widget, tk.Frame):
                for child_widget in widget.winfo_children():
                    if isinstance(child_widget, tk.Entry) and (child_widget != self.room_vars['x_room'] and child_widget != self.room_vars['y_room']):
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, ttk.Combobox) and (child_widget != self.room_vars['exit_type'] and child_widget != self.room_vars['wall']):
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, tk.Button) and child_widget['text'] == 'Validate':
                        child_widget.config(state='disabled')

    def disable_step3_widgets(self):
        # Disable step 3 widgets
        for widget in self.root.winfo_children():
            if isinstance(widget, tk.Frame):
                for child_widget in widget.winfo_children():
                    if isinstance(child_widget, tk.Entry) and child_widget != self.exit_coords_vars['exit_coords']:
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, tk.Button) and child_widget['text'] == 'Validate':
                        child_widget.config(state='disabled')

    def create_resizable_window(self):
        x_room = float(self.room_vars['x_room'].get())
        y_room = float(self.room_vars['y_room'].get())
        exit_type = self.room_vars['exit_type'].get()
        wall = self.room_vars['wall'].get()
        exit_coords_str = self.exit_coords_vars['exit_coords'].get()

        try:
            exit_coords = float(exit_coords_str)
        except ValueError:
            messagebox.showwarning(title="Error", message="Invalid input. Please enter a valid numeric value for exit coordinates.")
            return

        exit_coords_function = self.exit_coordinates(exit_type, exit_coords, x_room, y_room, wall)
        if exit_coords_function is not None:
            x0_exit, y0_exit, xf_exit, yf_exit = exit_coords_function
        else:
            return

        # Create the resizable window with the correct exit coordinates
        resizable_window = ResizableWindow(tk.Toplevel(self.root), x_room, y_room, (x0_exit, y0_exit, xf_exit, yf_exit), self.individual_speed, self.num_individuals)

        # Disable step 3 widgets after successful validation
        self.disable_step3_widgets()

    def exit_coordinates(self, exit_type, exit_coords, x_room, y_room, wall):
        exit_half_length = self.exit_dict[exit_type] / 2
        start_exit = exit_coords - exit_half_length
        end_exit = exit_coords + exit_half_length

        if wall == "Top" : 
            x0_exit, xf_exit = start_exit, end_exit
            y0_exit, yf_exit = 0, 0
        elif wall == "Bottom" : 
            x0_exit, xf_exit = start_exit, end_exit
            y0_exit, yf_exit = y_room, y_room
        elif wall == "Left" : 
            x0_exit, xf_exit = 0, 0
            y0_exit, yf_exit = start_exit, end_exit
        elif wall == "Right" : 
            x0_exit, xf_exit = x_room, x_room
            y0_exit, yf_exit = start_exit, end_exit
        else:
            return None

        return x0_exit, y0_exit, xf_exit, yf_exit

    def perform_simulation(self):
        # Create the resizable window and perform simulation logic here
        self.create_resizable_window()

class ResizableWindow:
    def __init__(self, master, rect_x_meters, rect_y_meters, exit_coords, individual_speed, nb_individuals):
        self.master = master
        self.master.title("Crowd simulation")

        self.create_widgets()

        self.rect_x_meters = rect_x_meters
        self.rect_y_meters = rect_y_meters
        self.exit_coords = exit_coords
        self.margin_px = 50

        # Initial canvas size
        self.initial_canvas_width = self.canvas_width
        self.initial_canvas_height = self.canvas_height

        self.simulation_speed = individual_speed
        self.nbSheep = nb_individuals

        self.seed = int(time.time())
        self.sheep_array = None  # To store the array of sheep
        self.start_time = None  # To store the start time

    def create_widgets(self):
        # Create a frame for the canvas
        canvas_frame = tk.Frame(self.master)
        canvas_frame.pack(fill=tk.BOTH, expand=True)

        # Set the desired canvas size (replace 1000 and 800 with your preferred values)
        # Set the canvas size to take nearly all the screen
        screen_width = self.master.winfo_screenwidth()
        screen_height = self.master.winfo_screenheight()
        self.canvas_width = int(screen_width * 0.8)
        self.canvas_height = int(screen_height * 0.8)  # the canvas size is 90% of the screen width and height

        self.canvas = tk.Canvas(canvas_frame, bg="white", width=self.canvas_width, height=self.canvas_height)
        self.canvas.pack(fill=tk.BOTH, expand=True)

        # Create a frame for buttons
        button_frame = tk.Frame(self.master)
        button_frame.pack()

        # Button to start the simulation
        self.start_button = tk.Button(button_frame, text="Start Simulation", command=self.start_simulation)
        self.start_button.pack()

        # Label for chronometer
        self.chronometer_label = tk.Label(button_frame, text="Elapsed Time: 0 seconds")
        self.chronometer_label.pack()

        self.canvas.bind("<Configure>", self.on_resize)

    def on_resize(self, event):
        # Calculate the scale factor for meters to pixels based on the initial canvas size
        scale_x = (self.initial_canvas_width - 2 * self.margin_px) / self.rect_x_meters
        scale_y = (self.initial_canvas_height - 2 * self.margin_px) / self.rect_y_meters
        scale_factor = min(scale_x, scale_y)

        # Update the sizes of elements based on the scale factor
        rect_x_px = self.rect_x_meters * scale_factor
        rect_y_px = self.rect_y_meters * scale_factor
        scale_length_px = 1.0 * scale_factor  # Assuming a fixed scale length for simplicity
        x0_exit_px, y0_exit_px = self.exit_coords[0] * scale_factor + self.margin_px, self.exit_coords[1] * scale_factor + self.margin_px
        xf_exit_px, yf_exit_px = self.exit_coords[2] * scale_factor + self.margin_px, self.exit_coords[3] * scale_factor + self.margin_px

        # Clear previous elements
        self.canvas.delete("all")

        # Draw rectangle
        self.canvas.create_rectangle(
            self.margin_px, self.margin_px, self.margin_px + rect_x_px, self.margin_px + rect_y_px, outline="black"
        )

        # Draw scale line
        scale_start_x = self.margin_px + rect_x_px - scale_length_px
        scale_start_y = self.margin_px + rect_y_px + 20
        scale_end_x = self.margin_px + rect_x_px
        scale_end_y = self.margin_px + rect_y_px + 20
        self.canvas.create_line(scale_start_x, scale_start_y, scale_end_x, scale_end_y, fill="black")

        # Draw small edges on the sides of the line
        edge_size = 4
        self.canvas.create_line(
            scale_start_x, scale_start_y - edge_size, scale_start_x, scale_start_y + edge_size, fill="black"
        )
        self.canvas.create_line(
            scale_end_x, scale_end_y - edge_size, scale_end_x, scale_end_y + edge_size, fill="black"
        )

        # Draw meter scale text
        self.canvas.create_text(scale_end_x, scale_end_y + 10, text="1m", anchor=tk.E)

        # Draw x and y axis
        x0_axis, y0_axis = self.margin_px / 2, self.margin_px / 2
        xf_axis, yf_axis = self.margin_px, self.margin_px
        self.canvas.create_line(x0_axis, y0_axis, xf_axis, y0_axis, arrow=tk.LAST, fill="blue")
        self.canvas.create_line(x0_axis, y0_axis, x0_axis, yf_axis, arrow=tk.LAST, fill="blue")
        self.canvas.create_text(xf_axis + 10, y0_axis, text="x", anchor=tk.W, fill="blue")
        self.canvas.create_text(x0_axis, yf_axis + 10, text="y", anchor=tk.W, fill="blue")

        # Draw exit
        self.canvas.create_line(x0_exit_px, y0_exit_px, xf_exit_px, yf_exit_px, fill="red")

        # Store calculated values in instance variables
        self.rect_x_px = rect_x_px
        self.rect_y_px = rect_y_px
        self.x0_exit_px = x0_exit_px
        self.y0_exit_px = y0_exit_px
        self.xf_exit_px = xf_exit_px
        self.yf_exit_px = yf_exit_px

    def start_simulation(self):
        self.sheep_array = None
        self.start_time = None

        # Disable the button after clicking
        self.start_button.config(state=tk.DISABLED)

        # Record the start time
        self.start_time = time.time()

        # Seed random generator
        my_library.seedRandomGenerator(self.seed)

        # Generate room and random sheeps
        #room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y = self.rect_x_px, self.rect_y_px, self.x0_exit_px, self.y0_exit_px, self.xf_exit_px, self.yf_exit_px
        #room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y = 500, 400, 500, 100, 500, 120
        #my_library.generateRoom(room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y)
        
        #my_library.generateRoom(self.rect_x_px , self.rect_y_px, self.x0_exit_px, self.y0_exit_px, self.xf_exit_px, self.yf_exit_px)

        # Generate room and random sheeps with adjusted room dimensions
        room_width = self.rect_x_px + 2 * self.margin_px
        room_height = self.rect_y_px + 2 * self.margin_px

        exit_start_x, exit_start_y, exit_end_x, exit_end_y = (
            self.x0_exit_px,
            self.y0_exit_px,
            self.xf_exit_px,
            self.yf_exit_px,
        )
        my_library.generateRoom(room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y) # tuez moi wesh PQ CA MARCHE PAS LJCN ZËOFH PZKDJNA ZLNDXPI1EJD ¨X1E2UHD 1XP   2A
        # j e d e v i e n s a g o r a p h o b e 

        sheep_array = my_library.generateRandomSheeps(self.nbSheep, 1.0)

        # Start the simulation loop
        self.simulate_movement()

    def simulate_movement(self):
        self.sheep_array 
        self.start_time
        # Call moveSheep function
        result = my_library.moveSheep(self.nbSheep)
        self.sheep_array = result

        # Clear previous sheep drawings
        self.canvas.delete("sheep")

        # Draw new sheep positions
        for i in range(self.nbSheep):
            x, y = result[i].x, result[i].y
            self.canvas.create_oval(x - 5, y - 5, x + 5, y + 5, fill="blue", tags="sheep")
        
        '''for i, sheep in enumerate(self.sheep_array): a
            x, y = sheep.x, sheep.y
            self.canvas.coords(self.sheep_objects[i], x, y, x + 5, y + 5)'''

        # Update the canvas
        self.master.update()

        # Free allocated memory using the correct type
        my_library.freeMatrix(ctypes.cast(result, ctypes.POINTER(Sheep)))

        # Check if any sheep is still inside, and schedule the next simulation step
        if my_library.sheepStillInside(self.nbSheep):
            # Calculate elapsed time
            elapsed_time = time.time() - self.start_time
            elapsed_time_str = f"Elapsed Time: {int(elapsed_time)} seconds"
            self.chronometer_label.config(text=elapsed_time_str)

            self.master.after(int(self.simulation_speed), self.simulate_movement)

if __name__ == "__main__": 
    root = tk.Tk()
    app = DataEntryForm(root)
    root.mainloop()

