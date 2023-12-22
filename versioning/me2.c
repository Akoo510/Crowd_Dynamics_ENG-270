#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#define ROOM_WIDTH 40
#define ROOM_HEIGHT 20
#define EXIT_SIZE 4
#define NUM_PEOPLE 70

typedef struct {
    int x, y;
} Person;

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point parent;
    double f, g, h;
} Node;

void clearScreen() {
    printf("\033[2J\033[1;1H");
}

double calculateHeuristic(Point current, Point goal) {
    // Simple Manhattan distance heuristic
    return fabs(current.x - goal.x) + fabs(current.y - goal.y);
}

int isCellOccupied(Person people[], int x, int y) {
    for (int i = 0; i < NUM_PEOPLE; i++) {
        if (people[i].x == x && people[i].y == y) {
            return 1; // Cell is occupied
        }
    }
    return 0; // Cell is not occupied
}

int isValid(Point point) {
    return point.x >= 0 && point.x < ROOM_WIDTH && point.y >= 0 && point.y < ROOM_HEIGHT;
}

int isDestination(Point current, Point destination) {
    return current.x == destination.x && current.y == destination.y;
}

void addToOpenList(Node* openList, int* openListCount, Point neighbor, Point goal, Point current) {
    double gNew = openList[current.y * ROOM_WIDTH + current.x].g + 1.0;
    double hNew = calculateHeuristic(neighbor, goal);
    double fNew = gNew + hNew;

    // Check if the neighbor is not in the open list or has a better path
    if (!openList[neighbor.y * ROOM_WIDTH + neighbor.x].f || fNew < openList[neighbor.y * ROOM_WIDTH + neighbor.x].f) {
        openList[neighbor.y * ROOM_WIDTH + neighbor.x].f = fNew;
        openList[neighbor.y * ROOM_WIDTH + neighbor.x].g = gNew;
        openList[neighbor.y * ROOM_WIDTH + neighbor.x].h = hNew;
        openList[neighbor.y * ROOM_WIDTH + neighbor.x].parent = current;
    }
}

void pathFinding(Person people[]) {
    for (int k = 0; k < NUM_PEOPLE; k++) {
        Point start = {people[k].x, people[k].y};
        Point goal = {ROOM_WIDTH - EXIT_SIZE, ROOM_HEIGHT / 2};

        Node openList[ROOM_HEIGHT * ROOM_WIDTH] = {0};
        int openListCount = 0;

        openList[start.y * ROOM_WIDTH + start.x].f = 0.0;
        openList[start.y * ROOM_WIDTH + start.x].g = 0.0;
        openList[start.y * ROOM_WIDTH + start.x].h = 0.0;

        int closedList[ROOM_HEIGHT * ROOM_WIDTH] = {0};

        while (1) {
            // Find the node with the lowest f in the open list
            double lowestF = DBL_MAX;
            int currentX, currentY;
            for (int i = 0; i < ROOM_HEIGHT * ROOM_WIDTH; i++) {
                int currentX = i % ROOM_WIDTH;
                int currentY = i / ROOM_WIDTH;
                printf("lol");
                for (int j = 0; j < ROOM_WIDTH; j++) {
                    if (openList[i].f > 0 && openList[i].f < lowestF) {
                        lowestF = openList[i * ROOM_WIDTH + j].f;
                        currentX = j;
                        currentY = i;
                    }
                }
            }

            // Move the current node to the closed list
            closedList[currentY * ROOM_WIDTH + currentX] = 1;


            // Check all 8 neighbors
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;

                    Point neighbor = {currentX + j, currentY + i};

                    // Check if the neighbor is within the room and not occupied
                    if (isValid(neighbor) && !isCellOccupied(people, neighbor.x, neighbor.y) && !closedList[neighbor.y * ROOM_WIDTH + neighbor.x]) {
                        // Check if the neighbor is the goal
                        if (isDestination(neighbor, goal)) {
                            // Update the position to the first step towards the goal
                            people[k].x = openList[neighbor.y * ROOM_WIDTH + neighbor.x].parent.x;
                            people[k].y = openList[neighbor.y * ROOM_WIDTH + neighbor.x].parent.y;
                            return;
                        }

                        // Add the neighbor to the open list
                        addToOpenList(&openList[0], &openListCount, neighbor, goal, start);
                    }
                }
            }

            // If the open list is empty and the goal is not reached, break
            if (openListCount == 0) {
                break;
            }
        }
    }
}

void drawRoom(Person people[]) {
    for (int i = 0; i < ROOM_HEIGHT; i++) {
        for (int j = 0; j < ROOM_WIDTH; j++) {
            if (i == 0 || i == ROOM_HEIGHT - 1) {
                printf("#");
            } else if (j == 0 || j == ROOM_WIDTH - 1) {
                printf("#");
            } else {
                int personDrawn = 0;
                for (int k = 0; k < NUM_PEOPLE; k++) {
                    if (people[k].x == j && people[k].y == i) {
                        printf("O");
                        personDrawn = 1;
                        break;
                    }
                }
                if (!personDrawn) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

int main() {
    Person people[NUM_PEOPLE];
    srand(time(NULL));

    for (int i = 0; i < NUM_PEOPLE; i++) {
        people[i].x = 1 + rand() % (ROOM_WIDTH - 2);
        people[i].y = 1 + rand() % (ROOM_HEIGHT - 2);
    }

    for (int i = 0; i < 40; i++) {
        //clearScreen();
        pathFinding(people);
        drawRoom(people);
        usleep(500000);
    }
    return 0;
}