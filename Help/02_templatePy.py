from ctypes import * # tt est surligné en jaune mais balec c'est dans l'étoile
import tkinter as tk

class Sheep(Structure):
    _fields_ = [("id", c_int), ("x", c_double), ("y", c_double), ("r", c_double)] # ici c'est pour remplir la structure donnée dans le c file

# On telecharge la librairie en C
so_file = r"C:\Users\tomje\OneDrive\EPFL\BA3\CMT\Pro-cul-feur\01_templateC.so"
bots_lib = CDLL(so_file) # on appelle la fonction c en appelant la librairie crée avec la compilation

# Creation de la window Tkinter 
root = tk.Tk()
root.title("Template GUI")
root.geometry("400x400")

# Creation du canva TkInter
canvas = tk.Canvas(root, width=400, height=400, bg="white")
canvas.pack()

# On définit cb de points on veut
num_bots = 1
bots = (Sheep * num_bots)() # on définit <num_dots> structures "point" avec chacunes des coordonnées random différentes
                            # c'est la fonction c qui remplit les structures avec des random coordinates

# Fonction qui genee les positions initiales des points (random) :
def generate_bots():
    bots_lib.generateRandomSheeps(bots, num_bots) #on appelle la fonction c là (dans la librairie .so) 
    for dot in bots:
        canvas.create_oval(dot.x, dot.y, dot.x + 5, dot.y + 5, fill="black") # On a defini une taille de base, mais dans le vrai code la taille d'un bot peut varier (l'utilisateur choisit)


# On fait bouger nos loulous
def move_bots():
    bots_lib.moveSheep(bots, num_bots)
    canvas.delete("all")  # ca donne une impression de dynamique, trop swag
    for dot in bots:
        canvas.create_oval(dot.x, dot.y, dot.x + 5, dot.y + 5, fill="black") # pareil que dans l'autre fonction pour la taille des points
    root.after(50, move_bots) #on fait repartir la fonction encore et encore

# Let's GO
generate_bots()
move_bots()
root.mainloop()


