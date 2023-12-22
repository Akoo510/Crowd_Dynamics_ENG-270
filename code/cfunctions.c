// Include from the start
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define LEFT_TRESHOLD 0.2
#define RIGHT_TRESHOLD 0.8

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

/// @brief Calculate on which side of the room is the door exit
/// @return 0 : Left-side, 1 : Down-side, 2 : Right-side, 3 : Up-side
int roomExitSide()
{
    if (room.exitRoom.start.x == 0 && room.exitRoom.end.x == 0)
        return 0;
    if (room.exitRoom.start.y == room.sizeRoom.x && room.exitRoom.end.y == room.sizeRoom.x)
        return 1;
    if (room.exitRoom.start.x == room.sizeRoom.x && room.exitRoom.end.x == room.sizeRoom.x)
        return 2;
    if (room.exitRoom.start.y == 0 && room.exitRoom.end.y == 0)
        return 3;

    // If none of the above conditions are met, then it should be impossible to reach this point
    return -1;
}

/// @brief Calculates the force applied on the sheep so that it goes to the exit, and returns it normalized
/// @param sheep Struct in which there'is a given x, y, radius, struct Force component, inRoom (bollean)
void calculateForceTowardsExit(struct Sheep *sheep)
{
    int exitSide = roomExitSide();
    double dx = room.exitRoom.end.x - room.exitRoom.start.x;
    double dy = room.exitRoom.end.y - room.exitRoom.start.y;
    struct Point closestPoint = {room.exitRoom.start.x + dx / 2.0, room.exitRoom.start.y + dy / 2.0};

    // If the sheep coordinates are between the dooor, they go to a "very far" location (like this we can see them exit)
    switch (exitSide)
    {
    case 0:
        if (sheep->y >= room.exitRoom.start.y && sheep->y <= room.exitRoom.end.y && sheep->x < (room.sizeRoom.x * LEFT_TRESHOLD))
        {
            closestPoint.x = -9 * room.sizeRoom.x;
        }
        break;
    case 1:
        if (sheep->x >= room.exitRoom.start.x && sheep->x <= room.exitRoom.end.x && sheep->y > (room.sizeRoom.y * RIGHT_TRESHOLD))
        {
            closestPoint.y = 10 * room.sizeRoom.y;
        }
        break;
    case 2:
        if (sheep->y >= room.exitRoom.start.y && sheep->y <= room.exitRoom.end.y && sheep->x > (room.sizeRoom.x * RIGHT_TRESHOLD))
        {
            closestPoint.x = 10 * room.sizeRoom.x;
        }
        break;
    case 3:
        if (sheep->x >= room.exitRoom.start.x && sheep->x <= room.exitRoom.end.x && sheep->y < (room.sizeRoom.y * LEFT_TRESHOLD))
        {
            closestPoint.y = -9 * room.sizeRoom.y;
        }
        break;
    default:
        break;
    }

    dx = closestPoint.x - sheep->x;
    dy = closestPoint.y - sheep->y;
    double distance = sqrt(dx * dx + dy * dy);

    // Normalized force vector to exit
    sheep->force.fx = dx / distance;
    sheep->force.fy = dy / distance;
}

/// @brief Calculates the force that pushes the sheep away from the other sheep
/// @param sheep1 A single element of the sheep struct
/// @param sheep2 Another struct element to compare to
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

        // Checks if the new direction is opposite to the current direction
        // To avoid the sheep going backwards
        if ((new_fx * sheep1->force.fx + new_fy * sheep1->force.fy) < 0)
        {
            // If it is, set the force to zero
            new_fx = 0;
            new_fy = 0;
        }

        // Add the new force to the current force
        sheep1->force.fx -= new_fx;
        sheep1->force.fy -= new_fy;
    }
}

/// @brief Updates the position of all the sheep with respect to the different forces present (takes into account the sheep around and the coordinate of the exit)
/// @param sheepArray the matrix of sheeps position and other parameter of the struct
/// @param nbSheep number of sheep in the room initially
void updateForces(struct Sheep *sheepArray, int nbSheep)
{
    for (int i = 0; i < nbSheep; ++i)
    {
        if (sheepArray[i].inRoom)
        {
            calculateForceTowardsExit(&sheepArray[i]);
            for (int j = 0; j < nbSheep; ++j)
            {
                if (i != j && sheepArray[j].inRoom)
                {
                    calculateForceBetweenSheep(&sheepArray[i], &sheepArray[j]);
                }
            }
        }
    }
}

/// @brief Checks if the sheep is still inside the room, and if not, updates the force so that it does not collide with the wall
void handleExitSide0(struct Sheep *sheep, double newX)
{
    if (!(sheep->y > room.exitRoom.start.y && sheep->y < room.exitRoom.end.y) && (newX - sheep->r <= 0))
        if (newX - sheep->r == 0)
            sheep->force.fx = 0;
        else
            sheep->force.fx = -sheep->x + sheep->r;
    if ((sheep->x + sheep->r) < 0)
        sheep->inRoom = false;
}

/// @brief Checks if the sheep is still inside the room, and if not, updates the force so that it does not collide with the wall
void handleExitSide1(struct Sheep *sheep, double newY)
{
    if (!(sheep->x > room.exitRoom.start.x && sheep->x < room.exitRoom.end.x) && (newY + sheep->r >= room.sizeRoom.y))
        if (newY + sheep->r == room.sizeRoom.y)
            sheep->force.fy = 0;
        else
            sheep->force.fy = room.sizeRoom.y - (sheep->y + sheep->r);
    if ((sheep->y - sheep->r) > room.sizeRoom.y)
        sheep->inRoom = false;
}

/// @brief Checks if the sheep is still inside the room, and if not, updates the force so that it does not collide with the wall
void handleExitSide2(struct Sheep *sheep, double newX)
{
    if (!(sheep->y > room.exitRoom.start.y && sheep->y < room.exitRoom.end.y) && (newX + sheep->r >= room.sizeRoom.x))
        if (newX + sheep->r == room.sizeRoom.x)
            sheep->force.fx = 0;
        else
            sheep->force.fx = room.sizeRoom.x - (sheep->x + sheep->r);
    if ((sheep->x - sheep->r) > room.sizeRoom.x)
        sheep->inRoom = false;
}

/// @brief Checks if the sheep is still inside the room, and if not, updates the force so that it does not collide with the wall
void handleExitSide3(struct Sheep *sheep, double newY)
{
    if (!(sheep->x > room.exitRoom.start.x && sheep->x < room.exitRoom.end.x) && (newY - sheep->r <= 0))
        if (newY - sheep->r == 0)
            sheep->force.fy = 0;
        else
            sheep->force.fy = -sheep->y + sheep->r;
    if ((sheep->y + sheep->r) < 0)
        sheep->inRoom = false;
}

/// @brief Updates the positions of all sheeps and checks for collisions with walls or other sheeps
/// @param nbSheep The number of Sheep
/// @return An array of coordinate X and Y (in the struct format : Point)
struct Point *moveSheep(int nbSheep, double margin)
{
    // Creates a matrix to return
    struct Point *coordinates = malloc(nbSheep * sizeof(struct Point));
    updateForces(globalStructeArray, nbSheep);

    // Update the position of all the sheep
    for (int i = 0; i < nbSheep; ++i)
    {
        struct Sheep *sheep = &globalStructeArray[i];
        double newX = sheep->x + sheep->force.fx;
        double newY = sheep->y + sheep->force.fy;

        // If the sheep is inside the room and close to a wall, we update its position so that it does not collide with it
        if (sheep->inRoom)
        {
            switch (roomExitSide())
            {
            case 0:
                handleExitSide0(sheep, newX);
                break;
            case 1:
                handleExitSide1(sheep, newY);
                break;
            case 2:
                handleExitSide2(sheep, newX);
                break;
            case 3:
                handleExitSide3(sheep, newY);
                break;
            default:
                break;
            }
        }

        // We update the position in the coordinate matrix which will be send to the python code
        coordinates[i].x = sheep->x += sheep->force.fx + margin;
        coordinates[i].y = sheep->y += sheep->force.fy + margin;
    }
    return coordinates;
}

/// @brief Generates person at random positions
/// @param nbBots Initial number of person in the room
/// @param radius Radius of the person in the room
void generateRandomSheeps(int nbBots, double radius)
{
    // Creates a dynamic array to store the sheeps positions
    globalStructeArray = malloc(nbBots * sizeof(struct Sheep));
    for (int i = 0; i < nbBots; ++i)
    {
        // Checks wether there's is already a sheep in the given coordinate
        // And tries other coordinates if a sheep is already present
        int overlap;
        do
        {
            overlap = 0;

            // The parameter after the rand() enables to not place sheep in wall
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

        // Finally we initialize all the other values
        globalStructeArray[i].r = radius;
        globalStructeArray[i].force.fx = 0;
        globalStructeArray[i].force.fy = 0;
        globalStructeArray[i].inRoom = true;
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

/// @brief Allows to free a Matrix
/// @param sheep Struct Sheep matrix to be freed
void freeMatrix(struct Sheep sheep[])
{
    free(sheep);
}

/// @brief Allows to free a Point type matrix
/// @param point Struct Point matrix to be freed
void freePoint(struct Point *point)
{
    free(point);
}