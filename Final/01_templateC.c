// Include from the start
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

// Definition of Structure
struct Force
{
    double fx;
    double fy;
};

struct Sheep
{
    double x, y, r;
    struct Force force;
    bool inRoom;
};

struct Point
{
    double x, y;
};

struct Line
{
    struct Point start;
    struct Point end;
};

struct Room
{
    struct Line exitRoom;
    struct Point sizeRoom;
};

struct Room room;

struct Sheep *globalStructeArray = NULL;

/// @brief Sets a random time for the function srand()
/// @param seed Parameter which we get from the python code
void seedRandomGenerator(unsigned int seed)
{
    srand(seed);
}

// Calculates the force that pushes the sheep towards the exit

/// @brief Calculates the force applied on the sheep so that a goes to the exit, and returns it normalized
/// @param sheep Struct in which there'is a given x, y, radius, struct Force component, inRoom (bollean)
void calculateForceTowardsExit(struct Sheep *sheep)
{
    double dx = room.exitRoom.end.x - room.exitRoom.start.x;
    double dy = room.exitRoom.end.y - room.exitRoom.start.y;
    double t = ((sheep->x - room.exitRoom.start.x) * dx + (sheep->y - room.exitRoom.start.y) * dy) / (dx * dx + dy * dy);
    t = (t < 0) ? 0 : (t > 1) ? 1
                              : t;
    struct Point closestPoint = {room.exitRoom.start.x + t * dx, room.exitRoom.start.y + t * dy};

    // If the sheep coordinate are between the dooor, they go to a "very far" location
    if (sheep->y >= room.exitRoom.start.y && sheep->y <= room.exitRoom.end.y)
    {
        closestPoint.x = 2 * room.sizeRoom.x; // Set the x-coordinate to 2 times the room width
    }

    dx = closestPoint.x - sheep->x;
    dy = closestPoint.y - sheep->y;
    double distance = sqrt(dx * dx + dy * dy);

    // The force is proportional to the distance to the exit
    sheep->force.fx = dx / distance;
    sheep->force.fy = dy / distance;
}

// Calculates the force that pushes the sheep away from the other sheep
void calculateForceBetweenSheep(struct Sheep *sheep1, struct Sheep *sheep2)
{
    double dx = sheep2->x - sheep1->x;
    double dy = sheep2->y - sheep1->y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance < (sheep1->r + sheep2->r))
    {
        // Calculate repulsive force and add it to the current force
        double force = (sheep1->r + sheep2->r - distance) / distance;
        sheep1->force.fx -= force * dx;
        sheep1->force.fy -= force * dy;
    }
}

// Updates the position of a single sheep based on its velocity and acceleration
void updateForces(struct Sheep *sheepArray, int num_sheep)
{
    for (int i = 0; i < num_sheep; ++i)
    {
        if (sheepArray[i].inRoom)
        {
            calculateForceTowardsExit(&sheepArray[i]);
            for (int j = 0; j < num_sheep; ++j)
            {
                if (i != j && sheepArray[j].inRoom)
                {
                    calculateForceBetweenSheep(&sheepArray[i], &sheepArray[j]);
                }
            }
        }
    }
}

// Updates the positions of all sheeps and checks for collisions with walls or other sheeps

/// @brief Updates the positions of all sheeps and checks for collisions with walls or other sheeps
/// @param nbSheep The number of person
/// @return An array of coordinate X and Y (in the struct format : Point)
struct Point *moveSheep(int nbSheep) // POUR APRüS VISU : struct Point *fonction lololililolililol
{
    struct Point *coordinates = malloc(nbSheep * sizeof(struct Point));
    updateForces(globalStructeArray, nbSheep);
    for (int i = 0; i < nbSheep; ++i)
    {
        struct Sheep *sheep = &globalStructeArray[i];
        if (!sheep->inRoom)
            continue;
        sheep->x += sheep->force.fx;
        sheep->y += sheep->force.fy;
        if ((sheep->x + sheep->r) >= room.sizeRoom.x)
            sheep->inRoom = false;
    }
    for (int i = 0; i < nbSheep; i++)
    {
        coordinates[i].x = globalStructeArray[i].x;
        coordinates[i].y = globalStructeArray[i].y;
    }
    return coordinates;
}

/// @brief Generates person at random positions
/// @param nbBots Initial number of person in the room
/// @param radius Radius of the person inthe room
void generateRandomSheeps(int nbBots, double radius)
{
    globalStructeArray = malloc(nbBots * sizeof(struct Sheep));
    for (int i = 0; i < nbBots; ++i)
    {
        // avoid sheep
        int overlap;
        do
        {
            overlap = 0;
            globalStructeArray[i].x = radius + (double)rand() / RAND_MAX * (room.sizeRoom.x - 2 * radius);
            globalStructeArray[i].y = radius + (double)rand() / RAND_MAX * (room.sizeRoom.y - 2 * radius);
            for (int j = 0; j < i; ++j)
            {
                double dx = globalStructeArray[i].x - globalStructeArray[j].x;
                double dy = globalStructeArray[i].y - globalStructeArray[j].y;
                double distance = sqrt(dx * dx + dy * dy);
                if (distance < 2 * radius)
                {
                    overlap = 1;
                    break;
                }
            }
        } while (overlap);
        globalStructeArray[i].r = radius;
        globalStructeArray[i].force.fx = 0;
        globalStructeArray[i].force.fy = 0;
        globalStructeArray[i].inRoom = true;
        printf("x1: %f, y1: %f\n", globalStructeArray[i].x, globalStructeArray[i].y);
    }
}

/// @brief Initialize the room struct
/// @param width of the room
/// @param height of the room
/// @param startX of the exit
/// @param startY of the exit
/// @param endX of the exit
/// @param endY of the exit
void generateRoom(double width, double height, double startX, double startY, double endX, double endY)
{
    struct Point start = {startX, startY};
    struct Point end = {endX, endY};
    struct Point size = {width, height};
    room.exitRoom.start = start;
    room.exitRoom.end = end;
    room.sizeRoom = size;
}

/// @brief Check wether a person is still inside or not
/// @param nbSheep
/// @return 1 if a person is still inside, 0 otherwise
int sheepStillInside(int nbSheep)
{
    for (int i = 0; i < nbSheep; i++)
    {
        if (globalStructeArray[i].inRoom)
            return 1;
    }
    printf("all sheep exited the room !!\n");
    return 0;
}

// DO NOT FORGET THISSSS
// pleasse, very important otherwise explosion

/// @brief Allows to free a Matrix
/// @param sheep Struct Sheep matrix to be freed
void freeMatrix(struct Sheep sheep[])
{
    free(sheep);
}

int main()
{
    int nbSheep = 50;
    seedRandomGenerator((unsigned int)time(NULL));
    generateRoom(400.0, 400.0, 400.0, 190.0, 400.0, 210.0);
    generateRandomSheeps(nbSheep, 1.0);
    while (sheepStillInside(nbSheep))
    {
        struct Point *result = moveSheep(nbSheep);
        
        //Renvoie toute les coordonnées des points
        /* for (int i = 0; i < nbSheep; i++)
        {
            printf("x %f, y %f\n", result[i].x, result[i].y);
        }
        free(result); */
    }
    freeMatrix(globalStructeArray);
    return 0;
}

/*
Compile with :
gcc -shared -o 01_templateC.so 01_templateC.c
*/

