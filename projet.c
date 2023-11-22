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

}obstacleCarre;

int pathFinding(sheep *individu, cage enclos){
    double dx = individu->posX - enclos.exitX;
    double dy = abs(individu->posY - enclos.exitY);
    double dist = sqrt(dx*dx + dy*dy);
    double formerDist = dist;
    dist -= individu->speed;
    double dx2 = dist*abs(dx)/formerDist;
    double dy2 = dist*dy/formerDist;
    individu->posX = enclos.exitX - dx2;
    if (dx >= 0) individu->posY = enclos.exitY - dy2;
    else if (dx < 0) individu->posY = enclos.exitY + dy2;
    return 0;
}


int main(void){

    cage test_1_cage = {100, 100, 100, 50, 20};
    sheep test_1_sheep = {0, 100, 1, 3};
    while((test_1_sheep.posX <= test_1_cage.exitX)){
        pathFinding(&test_1_sheep, test_1_cage);
        printf("(%f:%f)\n", test_1_sheep.posX, test_1_sheep.posY);
    }
    printf("libreeeeeeeeeeeeeeeeeeee");

    return 0;
}