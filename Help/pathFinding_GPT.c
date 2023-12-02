#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define NUM_AGENTS 100
#define MAX_X 800
#define MAX_Y 600
#define MAX_SPEED 2

typedef struct {
    int x;
    int y;
    int speed;
} Agent;

void initializeAgents(Agent agents[], int numAgents) {
    srand(time(NULL));
    for (int i = 0; i < numAgents; i++) {
        agents[i].x = rand() % MAX_X;
        agents[i].y = rand() % MAX_Y;
        agents[i].speed = rand() % MAX_SPEED + 1;
    }
}

void updateAgents(Agent agents[], int numAgents) {
    for (int i = 0; i < numAgents; i++) {
        agents[i].x += (rand() % 3 - 1) * agents[i].speed;
        agents[i].y += (rand() % 3 - 1) * agents[i].speed;

        agents[i].x = (agents[i].x + MAX_X) % MAX_X;
        agents[i].y = (agents[i].y + MAX_Y) % MAX_Y;
    }
}

void renderAgents(SDL_Renderer *renderer, Agent agents[], int numAgents) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white

    for (int i = 0; i < numAgents; i++) {
        SDL_RenderDrawPoint(renderer, agents[i].x, agents[i].y);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Crowd Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAX_X, MAX_Y, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Agent agents[NUM_AGENTS];
    initializeAgents(agents, NUM_AGENTS);

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
        SDL_RenderClear(renderer);

        updateAgents(agents, NUM_AGENTS);
        renderAgents(renderer, agents, NUM_AGENTS);

        SDL_Delay(16); // Cap frame rate to approximately 60 frames per second
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
