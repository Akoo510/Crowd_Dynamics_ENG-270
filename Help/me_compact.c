#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Wanted to implement an A* algorithm, found one on github but was of 500 lines (and quite unreadable + understandable)

#define ROOM_WIDTH 40
#define ROOM_HEIGHT 20
#define EXIT_SIZE 4
#define NUM_PEOPLE 200

typedef struct
{
    int x, y;
} Person;

void clearScreen()
{
    printf("\033[2J\033[1;1H");
}

int isCellOccupied(Person people[], int x, int y)
{
    for (int i = 0; i < NUM_PEOPLE; i++)
    {
        if (people[i].x == x && people[i].y == y)
        {
            return 0; // Cell is occupied
        }
    }
    return 1; // Cell is not occupied
}

double distanceToExit(int x, int y)
{
    double exitX = ROOM_WIDTH;
    double exitY = (ROOM_HEIGHT - 1) / 2.0;
    return sqrt(pow(exitX - x, 2) + pow(exitY - y, 2));
}

int hasReachedExit(Person person)
{
    int exitTop = (ROOM_HEIGHT - EXIT_SIZE) / 2;
    int exitBottom = (ROOM_HEIGHT - EXIT_SIZE) / 2 + EXIT_SIZE - 1;

    return person.x == ROOM_WIDTH && person.y >= exitTop && person.y <= exitBottom;
}

int allPeopleExited(Person people[])
{
    for (int k = 0; k < NUM_PEOPLE; k++)
    {
        if (people[k].x != 0 || people[k].y != 0)
        {
            return 0; // At least one person is still inside
        }
    }
    return 1; // All people have exited
}

void pathFinding(Person people[])
{
    for (int k = 0; k < NUM_PEOPLE; k++)
    {
        if (hasReachedExit(people[k]))
        {
            people[k].x = 0;
            people[k].y = 0;
        }
        else if (people[k].x > 0 && people[k].y > 0)
        {
            double distLeft = distanceToExit(people[k].x - 1, people[k].y);
            double distRight = distanceToExit(people[k].x + 1, people[k].y);
            double distUp = distanceToExit(people[k].x, people[k].y - 1);
            double distDown = distanceToExit(people[k].x, people[k].y + 1);

            if (distLeft <= distUp && distLeft <= distDown && distLeft <= distRight && isCellOccupied(people, people[k].x - 1, people[k].y))
            {
                people[k].x -= 1;
            }
            else if (distUp <= distDown && distUp <= distRight && isCellOccupied(people, people[k].x, people[k].y - 1))
            {
                people[k].y -= 1;
            }
            else if (distDown <= distRight && isCellOccupied(people, people[k].x, people[k].y + 1))
            {
                people[k].y += 1;
            }
            else if (distRight <= distLeft && isCellOccupied(people, people[k].x + 1, people[k].y) && (people[k].x + 1) < ROOM_WIDTH)
            {
                people[k].x += 1;
            }
            else if (people[k].y >= ((ROOM_HEIGHT - EXIT_SIZE) / 2) && people[k].y <= ((ROOM_HEIGHT - EXIT_SIZE) / 2 + EXIT_SIZE - 1) && isCellOccupied(people, people[k].x + 1, people[k].y))
            {
                people[k].x += 1;
            }
        }
    }
}

void drawRoom(Person people[])
{
    for (int i = 0; i < ROOM_HEIGHT; i++)
    {
        for (int j = 0; j < ROOM_WIDTH; j++)
        {
            // Draw top and bottom walls
            if (i == 0 || i == ROOM_HEIGHT - 1)
            {
                printf("#");
            }
            // Draw left walls
            else if (j == 0)
            {
                printf("#");
            }
            // Draw right walls with 'E' in the last column
            else if (j == ROOM_WIDTH - 1)
            {
                if (i >= ((ROOM_HEIGHT - EXIT_SIZE) / 2) && i <= ((ROOM_HEIGHT - EXIT_SIZE) / 2 + EXIT_SIZE - 1))
                {
                    printf("E");
                }
                else
                {
                    printf("#");
                }
            }
            // Draw people
            else
            {
                int personDrawn = 0;
                for (int k = 0; k < NUM_PEOPLE; k++)
                {
                    if (people[k].x == j && people[k].y == i)
                    {
                        printf("O");
                        personDrawn = 1;
                        break;
                    }
                }
                if (!personDrawn)
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

int main()
{
    Person people[NUM_PEOPLE - 1];
    srand(time(NULL));
    int i = 0;
    // Initialize people's positions
    while (i < NUM_PEOPLE)
    {
        int xCoo = 1 + rand() % (ROOM_WIDTH - 2); // Avoid placing people on walls
        int yCoo = 1 + rand() % (ROOM_HEIGHT - 2);
        if (isCellOccupied(people, xCoo, yCoo))
        {
            people[i].x = xCoo;
            people[i].y = yCoo;
            i++;
        }
    }

    while (!allPeopleExited(people))
    {
        clearScreen();
        pathFinding(people);
        drawRoom(people);
        usleep(50000);
    }
    return 0;
}