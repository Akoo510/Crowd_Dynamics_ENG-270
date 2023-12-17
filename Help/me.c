#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ROOM_WIDTH 40
#define ROOM_HEIGHT 20
#define EXIT_SIZE 4
#define NUM_PEOPLE 70

typedef struct {
    int x, y;
} Person;

void clearScreen() {
    printf("\033[2J\033[1;1H");
}


int isCellOccupied(Person people[], int x, int y) {
    for (int i = 0; i < NUM_PEOPLE; i++) {
        if (people[i].x == x && people[i].y == y) {
            return 0; // Cell is occupied
        }
    }
    return 1; // Cell is not occupied
}

void pathFinding(Person people[]) {
    for (int k = 0; k < NUM_PEOPLE; k++) {
        if (people[k].x == 0 && people[k].y == 0){
            people[k].x = 0;
            people[k].y = 0;
        }
        if (people[k].x > 0 && people[k].x < (ROOM_WIDTH - 1) && people[k].x && isCellOccupied(people, people[k].x + 1, people[k].y)) {
            people[k].x += 1;
        }
        if (people[k].y > 0 && people[k].y < ((ROOM_HEIGHT -EXIT_SIZE) / 2 + EXIT_SIZE / 2) && isCellOccupied(people, people[k].x, people[k].y + 1)) {
            people[k].y += 1;
        }
        if (people[k].y > 0 && people[k].y > ((ROOM_HEIGHT - EXIT_SIZE) / 2 + EXIT_SIZE / 2) && isCellOccupied(people, people[k].x, people[k].y - 1)) {
            people[k].y -= 1;
        }
        if (people[k].y >= ((ROOM_HEIGHT -EXIT_SIZE) / 2 + EXIT_SIZE/ 2) && people[k].y <= ((ROOM_HEIGHT - EXIT_SIZE) / 2 + EXIT_SIZE / 2) && people[k].x == (ROOM_WIDTH - 1)){
            people[k].x = 0;
            people[k].y = 0;
        }
    }
}

void drawRoom(Person people[]) {
    for (int i = 0; i < ROOM_HEIGHT; i++) {
        for (int j = 0; j < ROOM_WIDTH; j++) {
            // Draw top and bottom walls
            if (i == 0 || i == ROOM_HEIGHT - 1) {
                printf("#");
            }
            // Draw left and right walls
            else if (j == 0 || j == ROOM_WIDTH - 1) {
                printf("#");
            }
            // Draw people
            else {
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

    // Initialize people's positions
    for (int i = 0; i < NUM_PEOPLE; i++) {
        people[i].x = 1 + rand() % (ROOM_WIDTH - 2);  // Avoid placing people on walls
        people[i].y = 1 + rand() % (ROOM_HEIGHT - 2);
    }

    for (int i = 0; i < 40; i++) {
        /* for (int i = 0; i < NUM_PEOPLE; i++) {
            printf("People %d, x : %d, y : %d\n", i, people[i].x, people[i].y);
        } */
        clearScreen();
        pathFinding(people);
        drawRoom(people);
        usleep(500000);
    }

    return 0;
}
