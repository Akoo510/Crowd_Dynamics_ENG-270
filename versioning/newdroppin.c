#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct
{
    double x, y, r;
    double distExit;
} Sheep;

Sheep *sheepArray;

typedef struct 
{
    double x, y, size;
} Exit;

Exit roomExit;

typedef struct 
{
    double x, y;  
} Vector;

typedef struct
{
    int type; // 0 == Sheep ; 1 == Wall
    double x, y;

} Avoid;

typedef struct 
{
    double xLength, yHeight;
    int wallOfExit;
    // 0 == gauche, 1 == bas, 2 == droite, 3 == haut
    Exit ex;
    // Si 0 / 3 : depuis haut gauche
    // Si 1 / 2 : depuis en bas à droite
} Wall;

Wall walle;



double norm(double x, double y)
{
    return sqrt(x * x + y * y);
}

void delta(Vector *delta, double x1, double y1, double x2, double y2)
{
    delta->x = (x1 - x2);
    delta->y = (y1 - y2);
}

void exitVect(Vector *vector, Sheep sheep, Exit exit, double speed)
{
    vector->x = 0;
    vector->y = 0;
    delta(vector, exit.x, exit.y, sheep.x, sheep.y);
    double norme = norm(vector->x, vector->y);
    vector->x = vector->x/norme * speed;
    vector->y = vector->y/norme * speed;
    sheep.distExit = norme;
}

int inTheRadiusSheep(Vector deltaMove, Sheep sheepConcerned, Sheep sheepOther)
{
    Vector distance;
    delta(&distance, sheepConcerned.x + deltaMove.x, sheepConcerned.y + deltaMove.y, sheepOther.x, sheepOther.y);
    return (norm(distance.x, distance.y) < (sheepConcerned.r + sheepOther.r));
}

int inTheRadiusWall(Vector deltaMove, Sheep sheepConcerned, Wall wall, int whichWall)
{
    Vector distance;
    double newSheepPosX = sheepConcerned.x + deltaMove.x;
    double newSheepPosY = sheepConcerned.y + deltaMove.y;

    switch (whichWall)
    {
        case 0 : //Left wall
            delta(&distance, newSheepPosX, newSheepPosY, 0, newSheepPosY);
            break;
        case 1 : //Bottom wall
            delta(&distance, newSheepPosX, newSheepPosY, newSheepPosX, wall.yHeight);
            break;
        case 2 : //Right wall
            delta(&distance, newSheepPosX, newSheepPosY, wall.xLength, newSheepPosY);
            break;
        case 3 : //Down wall
            delta(&distance, newSheepPosX, newSheepPosY, newSheepPosX, 0);
            break;
    }
    return norm(distance.x, distance.y) < sheepConcerned.r;
}

void cornerWall(Vector deltaMove, Sheep sheepConcerned, Wall wall, Avoid avoidList[], int whichWall, int indiceAvoid)
{
    /* 
        indiceAvoid doit être passé comme une adresse pour pouvoir être changé
     */
    Vector distance[2];
    double newSheepPosX = sheepConcerned.x + deltaMove.x;
    double newSheepPosY = sheepConcerned.y + deltaMove.y;

    switch (whichWall)
    {
    case 0: // Left Wall
        delta(&distance[0], newSheepPosX, newSheepPosY, 0, wall.ex.y - wall.ex.size / 2.0);
        delta(&distance[1], newSheepPosX, newSheepPosY, 0, wall.ex.y + wall.ex.size / 2.0);
        if (norm(distance[0].x, distance[0].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = 0;
            avoidList[indiceAvoid].y = wall.ex.y - wall.ex.size / 2.0;
            indiceAvoid++;
        }
        if (norm(distance[1].x, distance[1].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = 0;
            avoidList[indiceAvoid].y = wall.ex.y + wall.ex.size / 2.0;
            indiceAvoid++;
        }
        break;

    case 1: // Bottom Wall
        delta(&distance[0], newSheepPosX, newSheepPosY, wall.ex.x - wall.ex.size / 2.0, wall.yHeight);
        delta(&distance[1], newSheepPosX, newSheepPosY, wall.ex.x + wall.ex.size / 2.0, wall.yHeight);
        if (norm(distance[0].x, distance[0].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = wall.ex.x - wall.ex.size / 2.0;
            avoidList[indiceAvoid].y = wall.yHeight;
            indiceAvoid++;
        }
        if (norm(distance[1].x, distance[1].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = wall.ex.x + wall.ex.size / 2.0;
            avoidList[indiceAvoid].y = wall.yHeight;
            indiceAvoid++;
        }
        break;

    case 2: // Right Wall
        delta(&distance[0], newSheepPosX, newSheepPosY, wall.xLength, wall.ex.y - wall.ex.size / 2.0);
        delta(&distance[1], newSheepPosX, newSheepPosY, wall.xLength, wall.ex.y + wall.ex.size / 2.0);
        if (norm(distance[0].x, distance[0].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = wall.xLength;
            avoidList[indiceAvoid].y = wall.ex.y - wall.ex.size / 2.0;
            indiceAvoid++;
        }
        if (norm(distance[1].x, distance[1].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = wall.xLength;
            avoidList[indiceAvoid].y = wall.ex.y + wall.ex.size / 2.0;
            indiceAvoid++;
        }
        break;

    case 3: // Top Wall
        delta(&distance[0], newSheepPosX, newSheepPosY, wall.ex.x - wall.ex.size / 2.0, 0);
        delta(&distance[1], newSheepPosX, newSheepPosY, wall.ex.x + wall.ex.size / 2.0, 0);
        if (norm(distance[0].x, distance[0].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = wall.ex.x - wall.ex.size / 2.0;
            avoidList[indiceAvoid].y = 0;
            indiceAvoid++;
        }
        if (norm(distance[1].x, distance[1].y) < sheepConcerned.r)
        {
            avoidList[indiceAvoid].type = 1;
            avoidList[indiceAvoid].x = wall.ex.x + wall.ex.size / 2.0;
            avoidList[indiceAvoid].y = 0;
            indiceAvoid++;
        }
        break;
    }
}

int insideTheDoor(Vector deltaMove, Sheep sheepConcerned, Wall wall, int whichWall)
{
    double newSheepPosX = sheepConcerned.x + deltaMove.x;
    double newSheepPosY = sheepConcerned.y + deltaMove.y;
    switch (whichWall)
    {
    case 0: // Left Wall
        return (newSheepPosY < (wall.ex.y + wall.ex.size / 2.0) && newSheepPosY > (wall.ex.y - wall.ex.size / 2.0));
    case 1: // Bottom Wall
        return (newSheepPosX < (wall.ex.x + wall.ex.size / 2.0) && newSheepPosX > (wall.ex.x - wall.ex.size / 2.0));
    case 2: // Right Wall
        return (newSheepPosY < (wall.ex.y + wall.ex.size / 2.0) && newSheepPosY > (wall.ex.y - wall.ex.size / 2.0));
    case 3: // Top Wall
        return (newSheepPosX < (wall.ex.x + wall.ex.size / 2.0) && newSheepPosX > (wall.ex.x - wall.ex.size / 2.0));
    }
}

void addToAvoidSheep(Vector deltaMove, Sheep sheepConcerned, Sheep sheepOther, Avoid avoidList[], int indiceAvoid)
{
    //Checks for only one sheep -> need to loop this function
    if (inTheRadiusSheep(deltaMove, sheepConcerned, sheepOther))
    {
        avoidList[indiceAvoid].type = 0;
        avoidList[indiceAvoid].x = sheepOther.x;
        avoidList[indiceAvoid].y = sheepOther.y;
    }
}

void addToAvoidWall(Vector deltaMove, Sheep sheepConcerned, Wall wall, Avoid avoidList[], int indiceAvoid)
{
    //Checks all wall at once -> need to execute once / sheep
    double newSheepPosX = sheepConcerned.x + deltaMove.x;
    double newSheepPosY = sheepConcerned.y + deltaMove.y;
    for (int i = 0; i < 4; i++)
    {
        if (inTheRadiusWall(deltaMove, sheepConcerned, wall, i))
        {
            if (wall.wallOfExit == i && insideTheDoor(deltaMove, sheepConcerned, wall, i))
            {
                cornerWall(deltaMove, sheepConcerned, wall, avoidList, i, indiceAvoid);
            }
            else
            {
                switch (i)
                {
                    case 0 : //Left wall
                        avoidList[indiceAvoid].type = 1;
                        avoidList[indiceAvoid].x = 0;
                        avoidList[indiceAvoid].y = newSheepPosY;
                        indiceAvoid++;
                        break;
                    case 1 : //Bottom wall
                        avoidList[indiceAvoid].type = 1;
                        avoidList[indiceAvoid].x = newSheepPosX;
                        avoidList[indiceAvoid].y = wall.yHeight;
                        indiceAvoid++;
                        break;
                    case 2 : //Right wall
                        avoidList[indiceAvoid].type = 1;
                        avoidList[indiceAvoid].x = wall.xLength;
                        avoidList[indiceAvoid].y = newSheepPosY;
                        indiceAvoid++;
                        break;
                    case 3 : //Top wall
                        avoidList[indiceAvoid].type = 1;
                        avoidList[indiceAvoid].x = newSheepPosX;
                        avoidList[indiceAvoid].y = 0;
                        indiceAvoid++;
                        break;
                }
            }
        }
    }
}

//To implement
int newton3(Vector deltaMove, Sheep sheepConcerned, Exit exit, Sheep sheep[], int sheepConcernedIndice, int nbSheep, double speed, Wall wall)
{
    Avoid avoidList[10] = {0};
    int indiceAvoid = 0;
    Vector *vecteurDeplacement;
    exitVect(vecteurDeplacement, sheepConcerned, exit, speed);
    for(int i = 0; i < nbSheep; i++)
    {
        if (i != sheepConcernedIndice)
        {
            addToAvoidSheep(*vecteurDeplacement, sheepConcerned, sheep[i], avoidList, indiceAvoid);
            if (avoidList[indiceAvoid].x != 0 || avoidList[indiceAvoid].y != 0)
            {
                indiceAvoid++;
            }
            
        }
    }

    //CONTINUER à IMPLEMENTER
    addToAvoidWall(*vecteurDeplacement, sheepConcerned, wall, avoidList, indiceAvoid);
    if (avoidList[indiceAvoid].x != 0 || avoidList[indiceAvoid].y != 0)
    {
        indiceAvoid++;
    }
    for (int i = 0; i < 10; i++)
    {
        if(avoidList[indiceAvoid].x != 0 || avoidList[indiceAvoid].y != 0)
        {
            vecteurDeplacement->x;
        }
    }

}



//To implement
Sheep *positionUpdate(int nbSheep)
{
    for(int i = 0; i < nbSheep; i++)
    {

    }
    return sheepArray;
}

int onWhichWall(double exitX, double exitY, double xLength, double yHeight)
{
    //Be careful with -1 indices in Python
    if (exitX == 0) return 0;
    if (exitY == yHeight) return 1;
    if (exitX == xLength) return 2;
    if (exitY == 0) return 3;
}

int initializeAll(double exitX, double exitY, double exitSize, double speed, int nbSheep, double speedSheep, double radiusSheep, double xLength, double yHeight)
{
    //Initialization of the room struct :
    roomExit.x = exitX;
    roomExit.y = exitY;
    roomExit.size = exitSize;

    //Initialization of stuct Wall:
    walle.xLength = xLength;
    walle.yHeight = yHeight;
    walle.wallOfExit = onWhichWall(exitX, exitY, xLength, yHeight);
    walle.ex = roomExit;

    //Initialization of sheep :
    srand(time(NULL));
    sheepArray = (Sheep *)malloc(nbSheep * sizeof(Sheep));
    Vector nul = {0.0, 0.0};
    for (int i = 0; i < nbSheep; i++)
    {
        sheepArray[i].x = (double)(rand() % (int)(xLength - 2.0 * radiusSheep));
        sheepArray[i].y = (double)(rand() % (int)(yHeight - 2.0 * radiusSheep));
        sheepArray[i].r = radiusSheep;
        // Check if the newly generated sheep collides with existing sheep
        for (int j = 0; j < i; j++)
        {
            if (inTheRadiusSheep(nul, sheepArray[i], sheepArray[j]))
            {
                // If there is a collision, re-generate coordinates for the current sheep
                i--;
                break;
            }
        }
    }
    return 0;
}

void freeMemory()
{
    // Free the allocated memory for the sheep array
    free(sheepArray);
}