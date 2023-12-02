#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROOM_WIDTH 10
#define ROOM_HEIGHT 5
#define MAX_PEOPLE 10
#define EXIT_X (ROOM_WIDTH)
#define EXIT_Y 3
/* #define EXIT_Y (rand() % ROOM_HEIGHT) // Random Y-coordinate on the right side */
#define EXIT_RADIUS 0.5 // Consider a range around the exit
#define OBSTACLE_WIDTH 3
#define OBSTACLE_X 5
#define OBSTACLE_Y 2
#define MIN_DISTANCE_BETWEEN_PEOPLE 0.9

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point position;
    int exited;
    double speed; // Speed attribute for each person
} Person;

Person people[MAX_PEOPLE];

void initializeSimulation(int numPeople, int roomWidth, int roomHeight) {
    for (int i = 0; i < numPeople; ++i) {
        people[i].exited = 0;
        people[i].speed = (rand() % 10) / 10.0 + 0.5; // Random speed between 0.5 and 1.4
    }
}

double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void movePeople() {
    for (int i = 0; i < MAX_PEOPLE; ++i) {
        if (!people[i].exited) {
            // Calculate the movement towards the exit using Pythagorean theorem
            double dx = EXIT_X - people[i].position.x;
            double dy = EXIT_Y - people[i].position.y;
            double distanceToExit = sqrt(dx * dx + dy * dy);

            // Adjust movement to avoid the obstacle
            if (people[i].position.x < OBSTACLE_X && people[i].position.y >= OBSTACLE_Y &&
                people[i].position.y < OBSTACLE_Y + OBSTACLE_WIDTH) {
                double obstacleDistanceX = OBSTACLE_X - people[i].position.x;
                double obstacleDistanceY = (people[i].position.y < OBSTACLE_Y) ?
                                            (OBSTACLE_Y - people[i].position.y) :
                                            (people[i].position.y - (OBSTACLE_Y + OBSTACLE_WIDTH));
                double obstacleDistance = sqrt(obstacleDistanceX * obstacleDistanceX + obstacleDistanceY * obstacleDistanceY);

                // If obstacle is too close, adjust movement
                if (obstacleDistance < distanceToExit) {
                    double scaleFactor = obstacleDistance / distanceToExit;
                    dx *= scaleFactor;
                    dy *= scaleFactor;
                }
            }

            // Update the position based on speed and direction
            if (distanceToExit > 0) {
                double scaleFactor = people[i].speed / distanceToExit;
                people[i].position.x += dx * scaleFactor;
                people[i].position.y += dy * scaleFactor;
            }

            // Add obstacle handling logic if needed
        }
    }
}


int checkExitConditions() {
    for (int i = 0; i < MAX_PEOPLE; ++i) {
        if (!people[i].exited &&
            calculateDistance(people[i].position.x, people[i].position.y, EXIT_X, EXIT_Y) < EXIT_RADIUS) {
            people[i].exited = 1;
        }

        for (int j = i + 1; j < MAX_PEOPLE; ++j) {
            double distanceBetweenPeople = calculateDistance(people[i].position.x, people[i].position.y,
                                                              people[j].position.x, people[j].position.y);

            if (distanceBetweenPeople < MIN_DISTANCE_BETWEEN_PEOPLE) {
                double scaleFactor = MIN_DISTANCE_BETWEEN_PEOPLE / distanceBetweenPeople;
                double dx = people[j].position.x - people[i].position.x;
                double dy = people[j].position.y - people[i].position.y;

                // Adjust positions to simulate a push
                double pushX = dx * (scaleFactor - 1) / 2;
                double pushY = dy * (scaleFactor - 1) / 2;

                // Adjust positions to stay within the room boundaries
                if (people[i].position.x + pushX >= 0 && people[i].position.x + pushX < ROOM_WIDTH) {
                    people[i].position.x += pushX;
                }
                if (people[i].position.y + pushY >= 0 && people[i].position.y + pushY < ROOM_HEIGHT) {
                    people[i].position.y += pushY;
                }

                if (people[j].position.x - pushX >= 0 && people[j].position.x - pushX < ROOM_WIDTH) {
                    people[j].position.x -= pushX;
                }
                if (people[j].position.y - pushY >= 0 && people[j].position.y - pushY < ROOM_HEIGHT) {
                    people[j].position.y -= pushY;
                }
            }
        }

        if (!people[i].exited) {
            if (people[i].position.x < 0) {
                people[i].position.x = 0;
            } else if (people[i].position.x >= ROOM_WIDTH) {
                people[i].position.x = ROOM_WIDTH - 1;
            }

            if (people[i].position.y < 0) {
                people[i].position.y = 0;
            } else if (people[i].position.y >= ROOM_HEIGHT) {
                people[i].position.y = ROOM_HEIGHT - 1;
            }
        }
    }

    // Check if everyone has exited
    for (int i = 0; i < MAX_PEOPLE; ++i) {
        if (!people[i].exited) {
            return 0; // Not everyone has exited
        }
    }

    return 1; // Everyone has exited
}

void printCurrentState() {
    for (int i = 0; i < MAX_PEOPLE; ++i) {
        printf("Person %d: (%.2lf, %.2lf)\n", i + 1, people[i].position.x, people[i].position.y);
    }
    printf("\n");
}

void runSimulation() {
    int simulationTime = 0;

    initializeSimulation(MAX_PEOPLE, ROOM_WIDTH, ROOM_HEIGHT);
    randomInitialization();

    while (!checkExitConditions()) {
        movePeople();
        printCurrentState();
        simulationTime++;
    }

    printf("Simulation completed in %d time steps.\n", simulationTime);
}

void randomInitialization() {
    for (int i = 0; i < MAX_PEOPLE; ++i) {
        people[i].position.x = (double)rand() / RAND_MAX * ROOM_WIDTH;
        people[i].position.y = (double)rand() / RAND_MAX * ROOM_HEIGHT;
    }
}

#include <Winsock2.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

void sendCoordinates(int clientSocket) {
    for (int i = 0; i < MAX_PEOPLE; ++i) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%.2lf %.2lf", people[i].position.x, people[i].position.y);

        send(clientSocket, buffer, strlen(buffer), 0);
        usleep(100);  // Introduce a small delay for demonstration purposes
    }
}

void runServer() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 1) < 0) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    socklen_t clientLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    sendCoordinates(clientSocket);

    close(clientSocket);
    close(serverSocket);
}

int main() {
    srand(time(NULL));
    runSimulation();
    runServer(); // Add this line to run the server after simulation
    return 0;
}

