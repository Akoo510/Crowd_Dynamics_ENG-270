import pygame
import sys
import ctypes
import os

""" First you need to run the following command in a C terminal :
gcc -fPIC -shared -o tests2.so tests2.c

It will crate a .so file which can be read by a python one
 """

path = os.getcwd()
clibrary = ctypes.CDLL(os.path.join(path, 'tests2.so'))


pathFinding = clibrary.findPath
pathFinding.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
pathFinding.restype = ctypes.POINTER(ctypes.c_int)
result = pathFinding(0, 0, 10, 5, 10 , 10) 
""" Argument :
posX, posY, doorX, doorY, roomX, roomY
 """

data = result[:2*11] #2*11 size of the matrix in C -> 2*(max(roomX or roomY)
clibrary.free_memory(result)

data_points = list(zip(data[0::2], data[1::2]))
for coordinate in data_points:
    print(coordinate)


""" Bon la suite marche pas pour le render """


# Initialize Pygame
pygame.init()

# Set up display
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Moving Point along Path")

# Define colors
white = (255, 255, 255)
black = (0, 0, 0)
red = (255, 0, 0)

# Your data points

# Function to draw the path
def draw_path(points):
    for i in range(len(points) - 1):
        pygame.draw.line(screen, white, points[i], points[i + 1], 2)

# Function to draw the moving point
def draw_moving_point(position):
    pygame.draw.circle(screen, red, position, 10)

# Main game loop
clock = pygame.time.Clock()
point_index = 0
moving_speed = 2

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Clear the screen
    screen.fill(black)

    # Draw the path
    draw_path(data_points)

    # Update the position of the moving point
    if point_index < len(data_points) - 1:
        moving_point = data_points[point_index]
        draw_moving_point(moving_point)

        # Move the point along the path
        point_index += moving_speed

    # Update the display
    pygame.display.flip()

    # Control the frame rate
    clock.tick(30)