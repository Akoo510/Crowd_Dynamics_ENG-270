#include <stdio.h>
#include <math.h>

/*
    LOIS :
    - Le points ne doivent pas avoir un rayon qui entre en colision
    - Les points veulent toujours aller vers la sortie
*/

typedef struct cage{
    double X;
    double Y;
    double exitX;
    double exitY;
    double exitSize;
}cage;


typedef struct sheep {
    double posX;
    double posY;
    double rayon;
    double speed;
}sheep;

typedef struct obstacleCarre {
    double posX1;
    double posY1;
    double posX2;
    double posY2;
    double posX3;
    double posY3;
    double posX4;
    double posY4;
}obstacleCarre;

int pathFinding(sheep *individu, cage enclos){
    double dx = individu->posX - enclos.exitX;
    double dy = individu->posY - enclos.exitY;
    double dist = sqrt(dx*dx + dy*dy);
    double formerDist = dist;
    dist -= individu->speed;
    double dx2 = dist*abs(dx)/formerDist;
    double dy2 = dist*abs(dy)/formerDist;
    individu->posX = enclos.exitX - dx2;
    if (dx >= 0) individu->posY = enclos.exitY + dx2;
    else if (dx < 0) individu->posY = enclos.exitY - dx2;

    if (dy >= 0) individu->posY = enclos.exitY + dy2;
    else if (dy < 0) individu->posY = enclos.exitY - dy2;
    return 0;
}

int isCollision(obstacleCarre obstacle, sheep individu, cage enclos){
    double sheepX = individu.posX;
    double sheepY = individu.posY;
    double edge1[2] = {obstacle.posX1, obstacle.posY1};
    double edge2[2] = {obstacle.posX2, obstacle.posY2};
    double edge3[2] = {obstacle.posX3, obstacle.posY3};
    double edge4[2] = {obstacle.posX4, obstacle.posY4};
    double exitX = enclos.exitX;
    double exitY = enclos.exitY;
    if (sheepX <= edge1[0] && sheepX <= exitX){
        //WSHHHHHH COMMENT Y A TROP DE CONDITION; FAUT FAIRE LES 4 EDGES ENFT, PAS QUE 2
    }
    
    return 0;
}

int whichEdge(obstacleCarre obstacle, sheep individu){
    if(individu.posX <= obstacle.posX1 && obstacle.posX1 <= enclos.exitX) || ();

    return 0;
}

int main(void){
    cage test_1_cage = {100, 100, 100, 50, 20};
    sheep test_1_sheep = {0, 0, 1, 3};
    while((test_1_sheep.posX <= test_1_cage.exitX)){
        pathFinding(&test_1_sheep, test_1_cage);
        printf("(%f:%f)\n", test_1_sheep.posX, test_1_sheep.posY);
    }
    printf("libreeeeeeeeeeeeeeeeeeee");

    return 0;
}