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


int pathFinding(sheep *individu, cage enclos){
    double dx = abs(individu->posX - enclos.exitX);
    double dy = abs(individu->posY - enclos.exitY);
    double dist = sqrt(dx*dx + dy*dy);
    double formerDist = dist;
    dist -= individu->speed;
    double dx2 = dist*dx/formerDist;
    double dy2 = dist*dy/formerDist;
    individu->posX = enclos.exitX - dx2;
    individu->posY = enclos.exitY - dy2;
    return 0;
}


int main(void){

    cage test_1_cage = {100, 100, 100, 50, 20};
    sheep test_1_sheep = {50, 50, 1, 1};
    //int dist_sheep_exit_X = abs(test_1_sheep.posX - test_1_cage.exitX);
    //int dist_sheep_exit_Y = abs(test_1_sheep.posY - test_1_cage.exitY);

    //double avancement_X->test_1_sheep.posX;
    //double avancement_Y = test_1_sheep.posY;
    while((test_1_sheep.posX <= test_1_cage.exitX)){
        pathFinding(&test_1_sheep, test_1_cage);
        printf("(%f:%f)\n", test_1_sheep.posX, test_1_sheep.posY);
    }

    return 0;
}