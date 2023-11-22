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


int pathFinding(double posSheepX, double posSheepY, double exitX, double exitY, double speed){
    double dx = abs(posSheepX - exitX);
    double dy = abs(posSheepY - exitY);
    double dist = sqrt(dx*dx + dy*dy);
    double formerDist = dist;
    dist -= speed;
    double dx2 = formerDist*dx/dist;
    double dy2 = formerDist*dy/dist;
    posSheepX = exitX - dx2;
    posSheepY = exitY - dy2;
    return 0;
}


int main(void){

    cage test_1_cage = {100, 100, 100, 50, 20};
    sheep test_1_sheep = {50, 50, 1};

    //int dist_sheep_exit_X = abs(test_1_sheep.posX - test_1_cage.exitX);
    //int dist_sheep_exit_Y = abs(test_1_sheep.posY - test_1_cage.exitY);

    double avancement_X = test_1_sheep.posX;
    double avancement_Y = test_1_sheep.posY;
    while((avancement_X != test_1_cage.exitX)&&(avancement_Y != test_1_cage.exitY)){
        pathFinding(avancement_X, avancement_Y, test_1_cage.exitX, test_1_cage.Y);
        printf("(%d : %d)\n", avancement_X, avancement_Y);
    }


    return 0;
}