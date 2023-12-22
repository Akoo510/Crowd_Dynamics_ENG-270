#include <stdio.h>
#include <stdlib.h>

// Function to find the path using Bresenham's Line Algorithm
int* findPath(int startX, int startY, int exitX, int exitY, int roomX, int roomY) {
    int *arr = malloc(((roomX >= roomY) ? roomX : roomY) * 2 * sizeof(int));
    int dx = abs(exitX - startX);
    int dy = abs(exitY - startY);

    int sx = (startX < exitX) ? 1 : -1;
    int sy = (startY < exitY) ? 1 : -1;

    int err = dx - dy;
    int i = 0;

    while ((startX != exitX || startY != exitY) && (startX >= 0 && startX < roomX && startY >= 0 && startY < roomY)) {
        arr[i] = startX;
        arr[i + 1] = startY;
        i = i + 2;

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
    arr[i + 1] = exitY;

    return arr;
}

void free_memory(int *arr) {
    free(arr);
}
