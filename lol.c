#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent a point in the grid
typedef struct {
    int x, y;
} Point;

// Node structure for A* algorithm
typedef struct {
    Point point;
    int g; // Cost from start node to current node
    int h; // Heuristic estimate from current node to goal
    int f; // Total cost: f = g + h
} AStarNode;

// Arrays to store open and closed lists
AStarNode openList[100 * 100];
AStarNode closedList[100 * 100];
int openListSize = 0;
int closedListSize = 0;

// Function to check if a point is inside the obstacle
bool isInsideObstacle(int x, int y, int obstacleX, int obstacleY, int obstacleSize) {
    return (x >= obstacleX && x < obstacleX + obstacleSize && y >= obstacleY && y < obstacleY + obstacleSize);
}

// Function to calculate the Manhattan distance between two points
int calculateDistance(Point p1, Point p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

// Function to check if a point is in the open list
bool isInOpenList(Point point) {
    for (int i = 0; i < openListSize; ++i) {
        if (openList[i].point.x == point.x && openList[i].point.y == point.y) {
            return true;
        }
    }
    return false;
}

// Function to check if a point is in the closed list
bool isInClosedList(Point point) {
    for (int i = 0; i < closedListSize; ++i) {
        if (closedList[i].point.x == point.x && closedList[i].point.y == point.y) {
            return true;
        }
    }
    return false;
}

// Function to add a node to the open list
void addToOpenList(AStarNode node) {
    openList[openListSize++] = node;
}

// Function to get the node with the lowest f value from the open list
AStarNode getLowestFNode() {
    int lowestFIndex = 0;
    for (int i = 1; i < openListSize; ++i) {
        if (openList[i].f < openList[lowestFIndex].f) {
            lowestFIndex = i;
        }
    }
    return openList[lowestFIndex];
}

// Function to remove a node from the open list
void removeFromOpenList(AStarNode node) {
    for (int i = 0; i < openListSize; ++i) {
        if (openList[i].point.x == node.point.x && openList[i].point.y == node.point.y) {
            openList[i] = openList[--openListSize];
            break;
        }
    }
}

// Function to add a node to the closed list
void addToClosedList(AStarNode node) {
    closedList[closedListSize++] = node;
}

// Function to find the most efficient path using A* algorithm
void findPath(Point start, Point destination, int obstacleX, int obstacleY, int obstacleSize, int roomWidth, int roomHeight) {
    // Reset open and closed lists
    openListSize = 0;
    closedListSize = 0;

    // ... (rest of the findPath function remains unchanged)
}

int main() {
    // Room dimensions
    int roomWidth = 100;
    int roomHeight = 100;

    // Door coordinates
    int doorX = 100;
    int doorY = 50;

    // Obstacle coordinates and size
    int obstacleX = 64;
    int obstacleY = 32;
    int obstacleSize = 3;

    // Starting point coordinates
    Point start = {20, 10};

    // Destination (door) coordinates
    Point destination = {doorX, doorY};

    // Find and print the most efficient path to the door, avoiding the obstacle
    findPath(start, destination, obstacleX, obstacleY, obstacleSize, roomWidth, roomHeight);

    return 0;
}
