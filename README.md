# Crowd Dynamics Simulation

## Project description
This repository contains C code for simulating the movement of sheep in a room. The simulation includes forces that drive the sheep towards an exit door while avoiding collisions with each other and the room walls.

## Project structure
[code/](./code) : This folder contains the program code
[bin/](./bin) : This folder contains the library created after compiling C code
[code/](./code) :


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
