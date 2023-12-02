#include <stdio.h>

char* display(char* str, int age){
    printf("Hello Worlddd, the year is %d, %s :)", age, str);
    return "Completed";
}

//Fonction à run pour créer une SO file.
//gcc -fPIC -shared -o connection.so connection.c 