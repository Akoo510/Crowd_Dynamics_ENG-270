#include <stdio.h>
#include <stdlib.h>

// Function to find the most efficient path using Bresenham's Line Algorithm
int* findPath(int startX, int startY, int exitX, int exitY, int roomX, int roomY) {
    int *arr = malloc(((roomX >= roomY)? roomX : roomY)*2*sizeof(int));
    int dx = abs(exitX - startX);
    int dy = abs(exitY - startY);

    int sx = (startX < exitX) ? 1 : -1;
    int sy = (startY < exitY) ? 1 : -1;

    int err = dx - dy;
    int i = 0;
    while (startX != exitX || startY != exitY) {
        arr[i] = startX;
        arr[i+1] = startY;
        i = i+2;
        //printf("(%d, %d)\n", startX, startY);
        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            startX += sx;
        }

        if (e2 < dx) {
            err += dx;
            startY += sy;
        }
    }
    arr[i] = exitX;
    arr[i+1] = exitY;
    return arr;
    // Print the destination point
    //printf("(%d, %d)\n", exitX, exitY);
}

void free_memory(int *arr){
    free(arr);
}