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

/// @brief Calculate on which side of the room is the door exit
/// @return 0 : Left, 1 : Down, 2 : Right, 3 : Up
int roomExitSide()
{
    if (room.exitRoom.start.x == 0 && room.exitRoom.end.x == 0) return 0;
    if (room.exitRoom.start.y == room.sizeRoom.x && room.exitRoom.end.y == room.sizeRoom.x) return 1;
    if (room.exitRoom.start.x == room.sizeRoom.x && room.exitRoom.end.x == room.sizeRoom.x) return 2;
    if (room.exitRoom.start.y == 0 && room.exitRoom.end.y == 0) return 3;

    // If none of the above conditions are met, then it should be impossible to reach this point
    return -1;
}

// Calculates the force that pushes the sheep towards the exit

/// @brief Calculates the force applied on the sheep so that a goes to the exit, and returns it normalized
/// @param sheep Struct in which there'is a given x, y, radius, struct Force component, inRoom (bollean)
void calculateForceTowardsExit(struct Sheep *sheep)
{
    int exitSide = roomExitSide();
    double dx = room.exitRoom.end.x - room.exitRoom.start.x;
    double dy = room.exitRoom.end.y - room.exitRoom.start.y;
    /* double t = ((sheep->x - room.exitRoom.start.x) * dx + (sheep->y - room.exitRoom.start.y) * dy) / (dx * dx + dy * dy);
    t = (t < 0) ? 0 : (t > 1) ? 1 : t; */
    struct Point closestPoint = {room.exitRoom.start.x + dx/2.0, room.exitRoom.start.y + dy / 2.0};

    // If the sheep coordinate are between the dooor, they go to a "very far" location
    
    switch (exitSide)
    {
    case 0:
        if (sheep->y >= room.exitRoom.start.y && sheep->y <= room.exitRoom.end.y && sheep->x < (room.sizeRoom.x * 0.2))
        {
            closestPoint.x = -2 * room.sizeRoom.x;
        }
        break;
    case 1:
        if (sheep->x >= room.exitRoom.start.x && sheep->x <= room.exitRoom.end.x && sheep->y > room.sizeRoom.y * 0.8)
        {
            closestPoint.y = 3 * room.sizeRoom.y;
        }
        break;
    case 2:
        if (sheep->y >= room.exitRoom.start.y && sheep->y <= room.exitRoom.end.y && sheep->x > (room.sizeRoom.x * 0.8))
        {
            closestPoint.x = 3 * room.sizeRoom.x;
        }
        break;
    case 3:
        if (sheep->x >= room.exitRoom.start.x && sheep->x <= room.exitRoom.end.x && sheep->y < (room.sizeRoom.y * 0.2))
        {
            closestPoint.y = -2 * room.sizeRoom.y;
        }
        break;
    default:
        break;
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
        // Calculate repulsive force
        double force = (sheep1->r + sheep2->r - distance) / distance;

        // Calculate new force components
        double new_fx = force * dx;
        double new_fy = force * dy;

        // Check if the new direction is opposite to the current direction
        if ((new_fx * sheep1->force.fx + new_fy * sheep1->force.fy) < 0)
        {
            // If it is, ignore the collision or handle it differently
            // For example, you could reduce the force or set it to zero
            new_fx = 0;
            new_fy = 0;
        }

        // Add the new force to the current force
        sheep1->force.fx -= new_fx;
        sheep1->force.fy -= new_fy;
    }
}

void calculateForceWall()
{

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
    int exitSide = roomExitSide();
    struct Point *coordinates = malloc(nbSheep * sizeof(struct Point));
    updateForces(globalStructeArray, nbSheep);
    
    for (int i = 0; i < nbSheep; ++i)
    {
        struct Sheep *sheep = &globalStructeArray[i];
        if (!sheep->inRoom)
            /* coordinates[i].x = 10.0;
            coordinates[i].y = 10.0; */
            continue;
        double newX = sheep->x + sheep->force.fx;
        double newY = sheep->y + sheep->force.fy;
        
        //Is the Sheep inside the room ?
        switch (exitSide)
        {
        case 0:
            if (!(sheep->y > room.exitRoom.start.y &&  sheep->y < room.exitRoom.end.y) && (newX - sheep->r <= 0))
                if (newX - sheep->r == 0) 
                    sheep->force.fx = 0;
                else
                    sheep->force.fx = sheep->x + sheep->r;
            if ((sheep->x + sheep->r) < 0)
                sheep->inRoom = false;
            break;
        case 1:
            if (!(sheep->x > room.exitRoom.start.x &&  sheep->x < room.exitRoom.end.x) && (newY + sheep->r >= room.sizeRoom.y))
                if (newY + sheep->r == room.sizeRoom.y)
                    sheep->force.fy = 0;
                else
                    sheep->force.fy = room.sizeRoom.y - (sheep->y + sheep->r);
            if ((sheep->y - sheep->r) > room.sizeRoom.y)
                sheep->inRoom = false;
            break;
        case 2:
            if (!(sheep->y > room.exitRoom.start.y &&  sheep->y < room.exitRoom.end.y) && (newX + sheep->r >= room.sizeRoom.x))
            if  (newX + sheep->r == room.sizeRoom.x)
                sheep->force.fx = 0;
            else 
                sheep->force.fx = room.sizeRoom.x - (sheep->x + sheep->r);
            if ((sheep->x - sheep->r) < room.sizeRoom.x)   
                sheep->inRoom = false;
            break;
        case 3:
            if (!(sheep->x > room.exitRoom.start.x &&  sheep->x < room.exitRoom.end.x) && (newY - sheep->r <= 0))
                if (newY - sheep->r == 0)
                    sheep->force.fy = 0;
                else
                    sheep->force.fy = sheep->y + sheep->r;
            if ((sheep->y + sheep->r) < 0)
                sheep->inRoom = false;
            break;
        default:
            break;
        }
        sheep->x += sheep->force.fx;
        sheep->y += sheep->force.fy;
        if (sheep->inRoom)
        {
            coordinates[i].x = globalStructeArray[i].x;
            coordinates[i].y = globalStructeArray[i].y;
        }
    }
    return coordinates;
}

/// @brief Generates person at random positions
/// @param nbBots Initial number of person in the room
/// @param radius Radius of the person in the room
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
        //printf("x1: %f, y1: %f\n", globalStructeArray[i].x, globalStructeArray[i].y);
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

