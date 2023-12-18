import ctypes

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
my_library = ctypes.CDLL(r"C:\Users\victo\OneDrive - epfl.ch\BA3\cmt\PROJET\tests persos\09_finalTom.so")  # Replace './09_finalTom.so' with the actual path to your shared library

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

# Example usage
def main():
    nbSheep = 7
    seed = 43

    # Seed random generator
    my_library.seedRandomGenerator(seed)

    # Generate room and random sheeps
    room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y = 400.0, 400.0, 400.0, 190.0, 400.0, 210.0
    my_library.generateRoom(room_width, room_height, exit_start_x, exit_start_y, exit_end_x, exit_end_y)
    my_library.generateRandomSheeps(nbSheep, 1.0)

    # Move sheeps until they all exit the room
    while my_library.sheepStillInside(nbSheep):
        # Call moveSheep function
        result = my_library.moveSheep(nbSheep)

        # Print the coordinates of the sheeps (uncomment if needed)
        # for i in range(nbSheep):
        #     print(f"x: {result[i].x}, y: {result[i].y}")

        # Free allocated memory using the correct type
        my_library.freeMatrix(ctypes.cast(result, ctypes.POINTER(Sheep)))

if __name__ == "__main__":
    main()
