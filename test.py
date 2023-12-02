import pygame
import sys
import math
import random

# Initialize pygame
pygame.init()

# Set up some constants
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
MARGIN = 10
CIRCLE_RADIUS = 10
CIRCLE_COLOR = (255, 255, 255)
BACKGROUND_COLOR = (0, 0, 0)

# Set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('Crowd Simulation')

# Set up the simulation
num_circles = 100
circles = []
for i in range(num_circles):
    x = random.randint(0, SCREEN_WIDTH - 2 * MARGIN) + MARGIN
    y = random.randint(0, SCREEN_HEIGHT - 2 * MARGIN) + MARGIN
    circles.append((x, y))

# Set up the game clock
clock = pygame.time.Clock()

# The main game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Draw the background
    screen.fill(BACKGROUND_COLOR)

    # Draw the circles
    for circle in circles:
        pygame.draw.circle(screen, CIRCLE_COLOR, circle, CIRCLE_RADIUS)

    # Calculate the new positions of the circles
    for i in range(num_circles):
        circle = circles[i]
        x, y = circle

        # Check if the circle is at the edge of the screen
        if x < MARGIN or x > SCREEN_WIDTH - MARGIN or y < MARGIN or y > SCREEN_HEIGHT - MARGIN:
            angle = random.uniform(0, 2 * math.pi)
            x = MARGIN + random.uniform(0, SCREEN_WIDTH - 2 * MARGIN)
            y = MARGIN + random.uniform(0, SCREEN_HEIGHT - 2 * MARGIN)
        else:
            angle = math.atan2(y - SCREEN_HEIGHT / 2, x - SCREEN_WIDTH / 2)
            distance = random.uniform(0, 5)
            x += math.cos(angle) * distance
            y += math.sin(angle) * distance

        # Update the position of the circle
        circles[i] = (x, y)

    # Update the display
    pygame.display.flip()

    # Delay to get 30 frames per second
    clock.tick(30)