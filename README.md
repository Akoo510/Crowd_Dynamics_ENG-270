# Crowd Dynamics Simulation

## Project description
This repository contains C code for simulating the movement of sheep in a room. The simulation includes forces that drive the sheep towards an exit door while avoiding collisions with each other and the room walls.

## Project structure
[code/](./code) : This folder contains the program code

[bin/](./bin) : This folder contains the library created after compiling C code

[versioning/](./versioning) : This folder contains all the past version, and different implmentation tried (as object avoidance for example)

## Implementation
* The calculation of the position is done by C and is then given to Python via a shared library that can be used by the `ctypes` module.
* Python actually renders the simulation with the use of `tkinter`
* A matric of coordinate x and y is passed from C to Python

## Instruction
To view the simulation run the Python code, it will :
* Compile the C code into a `.so` library
* Directly run the simulation once it's done

## Requirements

The code requires no instalation on python side, it can be run directcly (as `tkinter` is usually by default installed with python)

```
$ python --version
Python 3.11.5

$ gcc --version
gcc.exe (Rev2, Built by MSYS2 project) 13.2.0
```


## C file
### Features
#### Structures:

* Force: Represents a force vector with fx and fy components.
* Sheep: Describes a sheep with coordinates (x, y), radius (r), force (force), and a flag indicating whether it's in the room (inRoom).
* Point: Defines a point in 2D space with x and y coordinates.
* Line: Represents a line segment with start and end points.
* Room: Describes the simulation room, including the exit door and room size.

#### Functions:

* seedRandomGenerator: Seeds the random number generator.
* roomExitSide: Determines the side of the room where the exit door is located.
* calculateForceTowardsExit: Calculates the force applied to a sheep to move towards the exit door.
* calculateForceBetweenSheep: Computes the repulsive force between two sheep to avoid collisions.
* updateForces: Updates the position of sheep based on applied forces and interactions.
* handleExitSide0, handleExitSide1, handleExitSide2, handleExitSide3: Checks and updates forces to avoid collisions with room walls.
* moveSheep: Updates sheep positions considering forces and collisions, returns an array of coordinates.
* generateRandomSheeps: Generates random initial positions for sheep, avoiding overlaps.
* generateRoom: Initializes the room structure with specified dimensions and exit location.
* sheepStillInside: Checks if any sheep is still inside the room.
* freeMatrix: Frees memory allocated for the sheep matrix.

## Usage


Hwo to use C in python :
https://stackoverflow.com/questions/51856138/how-to-import-a-c-file-into-python
https://book.pythontips.com/en/latest/python_c_extension.html


orting array : https://www.geeksforgeeks.org/c-program-to-sort-an-array-in-ascending-order/
