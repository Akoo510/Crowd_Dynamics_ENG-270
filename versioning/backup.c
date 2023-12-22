#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


struct Force
{
    double fx;
    double fy;
};


struct Sheep {
    double x, y, r;
    struct Force force;
    bool inRoom;
};


struct Point
{
    double x, y;
};


struct Line {
    struct Point start;
    struct Point end;
};

struct Room
{
    struct Line exitRoom;
    struct Point sizeRoom;
};


struct Room room;


void seedRandomGenerator(unsigned int seed) {
    srand(seed);
}

// Calculates the force that pushes the sheep towards the exit
void calculateForceTowardsExit(struct Sheep *sheep) {
    double dx = room.exitRoom.end.x - room.exitRoom.start.x;
    double dy = room.exitRoom.end.y - room.exitRoom.start.y;
    double t = ((sheep->x - room.exitRoom.start.x) * dx + (sheep->y - room.exitRoom.start.y) * dy) / (dx * dx + dy * dy);
    t = (t < 0) ? 0 : (t > 1) ? 1 : t;
    struct Point closestPoint = {room.exitRoom.start.x + t * dx, room.exitRoom.start.y + t * dy};
    
    // If the sheep coordinate are between the dooor, they go to a "very far" location
    if (sheep->y >= room.exitRoom.start.y && sheep->y <= room.exitRoom.end.y) {
        closestPoint.x = 2 * room.sizeRoom.x;  // Set the x-coordinate to 2 times the room width
    }

    dx = closestPoint.x - sheep->x;
    dy = closestPoint.y - sheep->y; 
    double distance = sqrt(dx*dx + dy*dy);

    // The force is proportional to the distance to the exit
    sheep->force.fx = dx / distance;
    sheep->force.fy = dy / distance;
}


// Calculates the force that pushes the sheep away from the other sheep
void calculateForceBetweenSheep(struct Sheep *sheep1, struct Sheep *sheep2) 
{
    double dx = sheep2->x - sheep1->x;
    double dy = sheep2->y - sheep1->y;
    double distance = sqrt(dx*dx + dy*dy);

    if (distance < (sheep1->r + sheep2->r))
    {
        // Calculate repulsive force and add it to the current force
        double force = (sheep1->r + sheep2->r - distance) / distance;
        sheep1->force.fx -= force * dx;
        sheep1->force.fy -= force * dy;
    }
}


// Updates the position of a single sheep based on its velocity and acceleration
void updateForces(struct Sheep *sheepArray, int num_sheep) {
    for (int i = 0; i < num_sheep; ++i) {
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
void moveSheep(struct Sheep *sheepArray, int nbSheep) //POUR APRüS VISU : struct Sheep *fonction lololililolililol
{
    updateForces(sheepArray, nbSheep);
    for (int i = 0; i < nbSheep; ++i) {
        struct Sheep *sheep = &sheepArray[i];
        if (!sheep->inRoom) continue;
        sheep->x += sheep->force.fx;
        sheep->y += sheep->force.fy;
        if (sheep->x >= room.sizeRoom.x) sheep->inRoom = false;
    }
    //return sheepArray; //POUR LE DOCE APRüS, C'EST TROP COOL ENFT ZUMBAKAFEOUCRKANLAN DJBENS
}


void generateRandomSheeps(struct Sheep *sheepArray, int nbBots, double radius){
    for (int i = 0; i < nbBots; ++i) {
        //avoid sheep 
        int overlap;
        do {
            overlap = 0;
            sheepArray[i].x = radius + (double)rand() / RAND_MAX * (room.sizeRoom.x - 2 * radius);
            sheepArray[i].y = radius + (double)rand() / RAND_MAX * (room.sizeRoom.y - 2 * radius);
            for (int j = 0; j < i; ++j) {
                double dx = sheepArray[i].x - sheepArray[j].x;
                double dy = sheepArray[i].y - sheepArray[j].y;
                double distance = sqrt(dx*dx + dy*dy);
                if (distance < 2*radius) {
                    overlap = 1;
                    break;
                }
            }
        } while (overlap);
        sheepArray[i].r = radius;
        sheepArray[i].force.fx = 0;
        sheepArray[i].force.fy = 0;
        sheepArray[i].inRoom = true;
        printf("x1: %f, y1: %f\n", sheepArray[i].x, sheepArray[i].y);
    }
}


void generateRoom(double width, double height, double startX, double startY, double endX, double endY){
    struct Point start = {startX, startY};
    struct Point end = {endX, endY};
    struct Point size = {width, height};
    room.exitRoom.start = start;
    room.exitRoom.end = end;
    room.sizeRoom = size;
}

int sheepStillInside(struct Sheep *sheep, int nbSheep)
{
    for(int i = 0; i < nbSheep; i++)
    {
        if (sheep[i].inRoom) return 1;
    }
    printf("all sheep exited the room !!");
    return 0;
}


void freeMatrix(struct Sheep *sheep)
{
    free(sheep);
}

int main(){
    int nbSheep = 10;
    struct Sheep* sheepolata = malloc(nbSheep * sizeof(struct Sheep));

    seedRandomGenerator((unsigned int)time(NULL));
    generateRoom(400.0, 400.0, 400.0, 190.0, 400.0, 210.0);
    generateRandomSheeps(sheepolata, nbSheep, 1.0);
    while(sheepStillInside(sheepolata, nbSheep)){  
        updateForces(sheepolata, nbSheep);
        moveSheep(sheepolata, nbSheep);
    }
    for (int i = 0; i < nbSheep; i++){
        printf("x: %f, y: %f\n", sheepolata[i].x, sheepolata[i].y);
    }
    

    freeMatrix(sheepolata);
    

    /* generateRoom(400, 400, 400, 190, 400, 210);
    generateRandomSheeps(sheepolata, nbSheep, 1.0);

    while(allSheepExited(sheepolata, nbSheep))
    {
        updateForces(sheepolata, nbSheep);
        moveSheep(sheepolata, nbSheep);
    }

    freeMatrix(sheepolata); //DO NOT FORGET OTHERWISE ORDI WILL EXPLODE BOWWWWW */
    return 0;
}