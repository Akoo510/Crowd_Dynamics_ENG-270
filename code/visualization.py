##################### CMT PROJECT ###########################

# Tom Jeannin
# Victoria Véronique Marie Wattel

############# CROWD DYNAMICS SIMULATION #####################


## LIBRARIES ##

# For the GUI : 
import tkinter as tk
from tkinter import ttk, messagebox
from math import pi
import time

# For the shared library :
import subprocess
import ctypes

# For the absolute path :
import os
from pathlib import Path

## AUTOMATIC COMPILATION OF THE C CODE ##

def compile_c_code():
    parent_dir = Path(os.getcwd())
    compile_command = f"gcc -shared -o {parent_dir}/bin/cfunctions.so {parent_dir}/code/cfunctions.c"
    subprocess.run(compile_command, shell=True)

def load_shared_library():
    try:
        parent_dir = Path(os.getcwd())
        lib_path = f"{parent_dir}/bin/cfunctions.so"
        c_lib = ctypes.CDLL(lib_path)
        return c_lib
    except OSError as e:
        print(f"Error: Unable to load the shared library. Details: {e}")
        return None


# Compilation of the C code
compile_c_code()
my_library = load_shared_library()


## DEFINITION OF THE SHARED LIBRARY ELEMENTS ##

# Definition of the shared library structures in Python
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
    
# Definition of the shared library functions in Python
my_library.seedRandomGenerator.argtypes = [ctypes.c_uint]
my_library.generateRoom.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
my_library.generateRandomSheeps.argtypes = [ctypes.c_int, ctypes.c_double]
my_library.generateRandomSheeps.restype = ctypes.POINTER(Sheep)
my_library.sheepStillInside.argtypes = [ctypes.c_int]
my_library.sheepStillInside.restype = ctypes.c_int
my_library.moveSheep.argtypes = [ctypes.c_int]
my_library.moveSheep.restype = ctypes.POINTER(Point)
my_library.freePoint.argtypes = [ctypes.POINTER(Point)]


## DATA ENTRY FORM ##

class DataEntryForm:
    def __init__(self, data_entry_form_window):
        self.data_entry_form_window = data_entry_form_window
        self.data_entry_form_window.title("Data Entry Form")

        self.individual_speed = None
        self.num_individuals = None

        self.step = 1

        # Initialization of variables to store the user inputs
        self.individual_vars = {
            'num_individuals': tk.StringVar(value = "300"),
            'individual_type': tk.StringVar(value = "Human"),
            'individual_speed': tk.StringVar(value = "5"),
        }
        self.room_vars = {
            'x_room': tk.StringVar(value = "30.0"),
            'y_room': tk.StringVar(value = "30.0"),
            'exit_type': tk.StringVar(value = 'Double door'),
            'wall': tk.StringVar(value='Right'),
        }
        self.exit_coords_vars = {
            'exit_coords': tk.StringVar(value='15.0'),
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
        frame = tk.Frame(self.data_entry_form_window)
        frame.pack(padx=10, pady=10)

        nb_individuals_label = tk.Label(frame, text="Number of individuals wanted :")
        nb_individuals_entry = tk.Entry(frame, textvariable=self.individual_vars['num_individuals'])
        nb_individuals_label.grid(row=0, column=0, padx=5, pady=5)
        nb_individuals_entry.grid(row=0, column=1, padx=5, pady=5)

        type_individuals_label = tk.Label(frame, text="Individual type wanted :")
        type_individuals_combobox = ttk.Combobox(frame, values=['Human', 'Cow', 'Ant', 'Sheep', 'Elephant'],
                                                textvariable=self.individual_vars['individual_type'], state='readonly')
        type_individuals_label.grid(row=1, column=0, padx=5, pady=5)
        type_individuals_combobox.grid(row=1, column=1, padx=5, pady=5)

        speed_individuals_label = tk.Label(frame, text="Delay between each move [ms] :")
        speed_individuals_entry = tk.Entry(frame, textvariable=self.individual_vars['individual_speed'])
        speed_individuals_label.grid(row=2, column=0, padx=5, pady=5)
        speed_individuals_entry.grid(row=2, column=1, padx=5, pady=5)

        validate_button = tk.Button(frame, text="Validate", command=self.validate_step1)
        validate_button.grid(row=3, column=0, columnspan=2, pady=10)

    def create_step2_widgets(self):
        frame = tk.Frame(self.data_entry_form_window)
        frame.pack(padx=10, pady=10)

        x_room_label = tk.Label(frame, text="Choose the length of the room [m] :")
        x_room_entry = tk.Entry(frame, textvariable=self.room_vars['x_room'])
        x_room_label.grid(row=0, column=0, padx=5, pady=5)
        x_room_entry.grid(row=0, column=1, padx=5, pady=5)

        y_room_label = tk.Label(frame, text="Choose the height of the room [m] :")
        y_room_entry = tk.Entry(frame, textvariable=self.room_vars['y_room'])
        y_room_label.grid(row=1, column=0, padx=5, pady=5)
        y_room_entry.grid(row=1, column=1, padx=5, pady=5)

        exit_type_label = tk.Label(frame, text="Exit type wanted [m] :")
        exit_type_combobox = ttk.Combobox(frame, values=['Standard door', 'Double door', 'Ant nest hole', 'Paddock gate'],
                                          textvariable=self.room_vars['exit_type'], state='readonly')
        exit_type_label.grid(row=2, column=0, padx=5, pady=5)
        exit_type_combobox.grid(row=2, column=1, padx=5, pady=5)

        wall_label = tk.Label(frame, text="Wall wanted for the exit :")
        wall_combobox = ttk.Combobox(frame, values=['Right', 'Left', 'Top', 'Bottom'], textvariable=self.room_vars['wall'], state='readonly')
        wall_label.grid(row=3, column=0, padx=5, pady=5)
        wall_combobox.grid(row=3, column=1, padx=5, pady=5)

        validate_button = tk.Button(frame, text="Validate", command=self.validate_step2)
        validate_button.grid(row=4, column=0, columnspan=2, pady=10)

    def create_step3_widgets(self):
        frame = tk.Frame(self.data_entry_form_window)
        frame.pack(padx=10, pady=10)

        exit_coords_label = tk.Label(frame, text="Choose the length at which you want the exit to be on the wall [m] :")
        exit_coords_entry = tk.Entry(frame, textvariable=self.exit_coords_vars['exit_coords'])
        exit_coords_label.grid(row=0, column=0, padx=5, pady=5)
        exit_coords_entry.grid(row=0, column=1, padx=5, pady=5)

        validate_button = tk.Button(frame, text="Validate", command=self.validate_step3)
        validate_button.grid(row=1, column=0, columnspan=2, pady=10)

    def validate_step1(self):
        num_individuals = self.individual_vars['num_individuals'].get()
        individual_type = self.individual_vars['individual_type'].get()
        individual_speed = self.individual_vars['individual_speed'].get()

        if not num_individuals.isdigit() or int(num_individuals) == 0 or not individual_type or not individual_speed.isdigit() or int(individual_speed) == 0:
            messagebox.showwarning(title="Error", message="Invalid input. \nPlease enter valid, positive and non-zero integers.")
        else:
            # If validation is successful, we can move to the next step, disable step 1 widgets, and create step 2 widgets
            self.disable_step1_widgets()
            self.step += 1
            self.create_widgets()

    def validate_step2(self):
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
            individual_speed = int(individual_speed_str)
        except ValueError:
            messagebox.showwarning(title="Error", message="Invalid input. \nPlease enter valid numeric values\nRoom dimensions must be positive and non-zero floats.")
            return

        if not x_room > 0 or not y_room > 0 or not exit_type or not wall : #or not num_individuals > 0:
            messagebox.showwarning(title="Error", message="Invalid input. \nPlease enter valid values. \nRoom dimensions must be positive and non-zero floats.\nThe exit type must be selected.")
        elif self.radius_dict[individual_type]*2 > self.exit_dict[exit_type]:
            messagebox.showwarning(title="Error", message=f"Invalid input.\Chosen exit type must be greater than or equal to the diameter of the {individual_type}.\n Please select an adapted exit type. ")
        else:
            # Calculating the total area required for the individuals
            total_area_required = num_individuals * (pi * self.radius_dict[individual_type] ** 2)
            # Checking if the room area is sufficient
            if x_room * y_room < total_area_required:
                messagebox.showwarning(title="Error", message=f"Invalid input. \nRoom area is too small to accommodate all individuals.\n Reminder : \n Diameter of the {individual_type} = {pi * self.radius_dict[individual_type] ** 2}m. ")
            else:
            # Storing additional attributes for later use
                self.individual_speed = int(individual_speed)
                self.num_individuals = int(num_individuals)

                # If all conditions are met, we can move to the next step, disable step 2 widgets, and create step 3 widgets
                self.disable_step2_widgets()
                self.step += 1
                self.create_widgets()

    def validate_step3(self):
        exit_coords_str = self.exit_coords_vars['exit_coords'].get()
        try:
            exit_coords = float(exit_coords_str)
        except ValueError:
            messagebox.showwarning(title="Error", message="Invalid input. \nPlease enter a valid float for exit coordinates.")
            return

        # Validate additional condition for exit coordinates
        x_room = float(self.room_vars['x_room'].get())
        y_room = float(self.room_vars['y_room'].get())
        wall = self.room_vars['wall'].get()
        exit_half_length = self.exit_dict[self.room_vars['exit_type'].get()] / 2

        if wall == "Left" or wall == "Right":
            if not 0 <= exit_coords <= y_room:
                messagebox.showwarning(title="Error", message=f"Invalid input.\nExit coordinates must be between 0 and {y_room}(chosen room height).")
                return
            if not 0 + exit_half_length <= exit_coords <= y_room - exit_half_length:
                messagebox.showwarning(title="Error", message=f"Invalid input.\nThe chosen exit type exceeds the size of the {wall} wall at this position. \nIt must be at least at +/- {exit_half_length}m from the edge of the wall.")
                return
        elif wall == "Top" or wall == "Bottom":
            if not 0 <= exit_coords <= x_room:
                messagebox.showwarning(title="Error", message=f"Invalid input.\nExit coordinates must be between 0 and {x_room}(chosen room length).")
                return
            if not 0 + exit_half_length <= exit_coords <= x_room - exit_half_length:
                messagebox.showwarning(title="Error", message=f"The chosen exit type exceeds the size of the {wall} at this position. It must be at least {exit_half_length}m from the edge of the wall.")
                return

        # If all conditions are met, we can perform the simulation and show an info message
        self.perform_simulation()
        # Disabling step 3 widgets after successful validation
        self.disable_step3_widgets()
        # Showing info message after disabling step 3 widgets
        messagebox.showinfo(title="Data Entry Form completed", message="To start simulation, please close this information window and the Data Entry Form.")

    def disable_step1_widgets(self):
        # Disabling step 1 widgets
        for widget in self.data_entry_form_window.winfo_children():
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
        # Disabling step 2 widgets
        for widget in self.data_entry_form_window.winfo_children():
            if isinstance(widget, tk.Frame):
                for child_widget in widget.winfo_children():
                    if isinstance(child_widget, tk.Entry) and (child_widget != self.room_vars['x_room'] and child_widget != self.room_vars['y_room']):
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, ttk.Combobox) and (child_widget != self.room_vars['exit_type'] and child_widget != self.room_vars['wall']):
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, tk.Button) and child_widget['text'] == 'Validate':
                        child_widget.config(state='disabled')

    def disable_step3_widgets(self):
        # Disabling step 3 widgets
        for widget in self.data_entry_form_window.winfo_children():
            if isinstance(widget, tk.Frame):
                for child_widget in widget.winfo_children():
                    if isinstance(child_widget, tk.Entry) and child_widget != self.exit_coords_vars['exit_coords']:
                        child_widget.config(state='disabled')
                    elif isinstance(child_widget, tk.Button) and child_widget['text'] == 'Validate':
                        child_widget.config(state='disabled')

    def final_step_data_entry(self):
        x_room = float(self.room_vars['x_room'].get())
        y_room = float(self.room_vars['y_room'].get())
        exit_type = self.room_vars['exit_type'].get()
        wall = self.room_vars['wall'].get()
        exit_coords_str = self.exit_coords_vars['exit_coords'].get()

        try:
            exit_coords = float(exit_coords_str)
        except ValueError:
            messagebox.showwarning(title="Error", message="Invalid input.\n Please enter a valid numeric value for exit coordinates.")
            return

        exit_coords_function = self.exit_coordinates(exit_type, exit_coords, x_room, y_room, wall)
        if exit_coords_function is not None:
            x0_exit, y0_exit, xf_exit, yf_exit = exit_coords_function
        else:
            return
        
        # Disabling step 3 widgets after successful validation
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

    def get_room_dimensions(self):
        x_room = float(self.room_vars['x_room'].get())
        y_room = float(self.room_vars['y_room'].get())
        return x_room, y_room

    def get_simulation_parameters(self):
        num_individuals = self.num_individuals
        individual_speed = self.individual_speed
        individual_radius = self.radius_dict[self.individual_vars['individual_type'].get()]
        return num_individuals, individual_speed, individual_radius

    def get_exit_coordinates(self):
        x0_exit, y0_exit, xf_exit, yf_exit = self.exit_coordinates(
            self.room_vars['exit_type'].get(),
            float(self.exit_coords_vars['exit_coords'].get()),
            float(self.room_vars['x_room'].get()),
            float(self.room_vars['y_room'].get()),
            self.room_vars['wall'].get()
        )
        return x0_exit, y0_exit, xf_exit, yf_exit
    
    def perform_simulation(self):
        self.final_step_data_entry()

if __name__ == "__main__": 
    data_entry_form_window = tk.Tk()
    app = DataEntryForm(data_entry_form_window)
    data_entry_form_window.mainloop()

    x_room, y_room = app.get_room_dimensions()
    num_individuals, individual_speed, individual_radius = app.get_simulation_parameters()
    x0_exit, y0_exit, xf_exit, yf_exit = app.get_exit_coordinates()

    print("Room Dimensions:", x_room, y_room)
    print("Simulation Parameters:", num_individuals, individual_speed, individual_radius)
    print("Exit Coordinates:", x0_exit, y0_exit, xf_exit, yf_exit)

## DEFINITION OF THE SIMULATION ##

margin = 50
seed = int(time.time())
sheep_array = None 
start_time = None 

exit_length = max(xf_exit - x0_exit, yf_exit - y0_exit)

# Window dimensions (fixed values)
simulation_window = tk.Tk()
simulation_window.title("Crowd Movement Simulation")
simulation_window.geometry("1200x700")

# Canvas dimensions (fixed values)
canvas_width = 1100
canvas_height = 600

def find_scale_factor():
    scale_x = (canvas_width - 2 * margin) / x_room
    scale_y = (canvas_height - 2 * margin) / y_room
    scale_factor = min(scale_x, scale_y)
    return scale_factor

def find_px_length(size_in_meters, scale_factor):
    size_in_px = size_in_meters * scale_factor
    return size_in_px

scale_factor = find_scale_factor()
xRoom_px = find_px_length(x_room, scale_factor)
yRoom_px = find_px_length(y_room, scale_factor)
x0_exit_px = find_px_length(x0_exit, scale_factor)
y0_exit_px = find_px_length(y0_exit, scale_factor)
xf_exit_px = find_px_length(xf_exit, scale_factor)
yf_exit_px = find_px_length(yf_exit, scale_factor)
individual_radius_px = find_px_length(individual_radius, scale_factor)

 # We define a size of 1m for the scale
scale_px = find_px_length(1, scale_factor)

# Using the shared library functions 
def start_simulation():
    global sheep_array, start_time

    # Disabling the button after clicking
    button.config(state=tk.DISABLED)

    # Recording the start time
    start_time = time.time()

    # Seeding random generator
    my_library.seedRandomGenerator(seed)

    # Generating room and random sheeps
    my_library.generateRoom(xRoom_px, yRoom_px, x0_exit_px, y0_exit_px, xf_exit_px, yf_exit_px)
    sheep_array = my_library.generateRandomSheeps(num_individuals, individual_radius_px)

    # Starting the simulation loop
    simulate_movement()

def simulate_movement():
    global sheep_array, start_time

    result = my_library.moveSheep(num_individuals)

    # Clearing previous sheep drawings
    canvas.delete("sheep")

    # Drawing new sheep positions
    for i in range(num_individuals):
        x, y = result[i].x + margin, result[i].y + margin
        canvas.create_oval(x - individual_radius_px, y - individual_radius_px, x + individual_radius_px, y + individual_radius_px, fill="blue", tags="sheep")

    # Updating the canvas
    simulation_window.update()

    # Important ! : free allocated memory using the correct type
    my_library.freePoint(ctypes.cast(result, ctypes.POINTER(Point)))

    # Checking if any sheep is still inside
    if my_library.sheepStillInside(num_individuals):
        # Calculating elapsed time
        elapsed_time = time.time() - start_time
        elapsed_time_str = f"Elapsed Time: {int(elapsed_time)} seconds"
        chronometer_label.config(text=elapsed_time_str)

        # Scheduling the next simulation step
        simulation_window.after(individual_speed, simulate_movement)
    else:
        # Showing the message that all individuals have exited the room
        messagebox.showinfo("Simulation Complete", "All individuals have exited the room.")

# Creation of elements and frames in the window
        
# Canva frame : 
canvas_frame = tk.Frame(simulation_window, width=canvas_width, height=canvas_height, bg="lightblue")
canvas_frame.pack(side=tk.TOP, padx=10, pady=10)

canvas = tk.Canvas(canvas_frame, width=canvas_width, height=canvas_height, bg="white")
canvas.pack()

# Drawing elements on the canva
canvas.create_rectangle(margin, margin, margin + xRoom_px, margin + yRoom_px, outline="black", width=4)
canvas.create_line(x0_exit_px + margin, y0_exit_px + margin, xf_exit_px + margin, yf_exit_px + margin, fill="white", width=4)

scale_start_x = margin + xRoom_px - scale_px
scale_start_y = margin + yRoom_px + margin/2  # We set the scale to be at the middle of the margin 
scale_end_x = margin + xRoom_px
scale_end_y = margin + yRoom_px + margin/2  # We set the scale to be at the middle of the margin 
canvas.create_line(scale_start_x, scale_start_y, scale_end_x, scale_end_y, fill="black")
canvas.create_text(scale_end_x, scale_end_y + 10, text="1m", anchor=tk.E) # 10 = arbitrary value

edge_size = 4 # 4 = arbitrary value
canvas.create_line(scale_start_x, scale_start_y - edge_size, scale_start_x, scale_start_y + edge_size, fill="black")
canvas.create_line(scale_end_x, scale_end_y - edge_size, scale_end_x, scale_end_y + edge_size, fill="black")

x0_axis, y0_axis = margin/2, margin/2
xf_axis, yf_axis = margin, margin
canvas.create_line(x0_axis, y0_axis, xf_axis, y0_axis, arrow=tk.LAST, fill="blue")
canvas.create_line(x0_axis, y0_axis, x0_axis, yf_axis, arrow=tk.LAST, fill="blue")
canvas.create_text(xf_axis + 10, y0_axis, text="x", anchor=tk.W, fill="blue")
canvas.create_text(x0_axis, yf_axis + 10, text="y", anchor=tk.W, fill="blue")

# Button and elapsed time frame :
button_and_time_frame = tk.Frame(simulation_window, height=70)
button_and_time_frame.pack(side=tk.TOP, pady=10)

button = tk.Button(button_and_time_frame, text="Start simulation!", command=start_simulation)
button.pack(side=tk.TOP, anchor=tk.CENTER)

chronometer_label = tk.Label(button_and_time_frame, text="Elapsed Time: 0 seconds")
chronometer_label.pack(side=tk.BOTTOM, anchor=tk.CENTER)

# Simulation parameters frame :
parameters_frame = tk.Frame(simulation_window, width=50)
parameters_frame.pack(side=tk.BOTTOM, pady=1)

x_room_label = tk.Label(parameters_frame, text=f"Width = {x_room}m;")
x_room_label.grid(row=0, column=0, pady=1)
y_room_label = tk.Label(parameters_frame, text=f"Height = {y_room}m;")
y_room_label.grid(row=1, column=0, pady=1)
radius_label = tk.Label(parameters_frame, text=f"Individual Diameter = {individual_radius*2}m;")
radius_label.grid(row=2, column=0, pady=1)
speed_label = tk.Label(parameters_frame, text=f"Frame of the simulation = {individual_speed};")
speed_label.grid(row=0, column=1, pady=1)
num_individuals_label = tk.Label(parameters_frame, text=f"Number of individuals = {num_individuals};")
num_individuals_label.grid(row=1, column=1, pady=1)
exit_label = tk.Label(parameters_frame, text=f"Exit length = {exit_length}m;")
exit_label.grid(row=2, column=1, pady=1)

# Running the simulation 
simulation_window.mainloop()
