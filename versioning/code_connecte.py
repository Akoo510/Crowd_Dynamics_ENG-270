import tkinter as tk
import socket

class RoomGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.canvas = tk.Canvas(self.root, width=500, height=300)
        self.canvas.pack()

        self.connect_to_server()

        self.root.mainloop()

    def connect_to_server(self):
        server_address = ('localhost', 12345)  # Change to the actual server address
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect(server_address)

        self.update_coordinates(client_socket)

        client_socket.close()

    def update_coordinates(self, client_socket):
        while True:
            data = client_socket.recv(256).decode()
            if not data:
                break

            x, y = map(float, data.split())
            self.draw_person(x, y)

            self.root.update()  # Update the GUI

    def draw_person(self, x, y):
        self.canvas.create_oval(x * 50, y * 50, x * 50 + 5, y * 50 + 5, fill="red")  # Adjust scale as needed

if __name__ == "__main__":
    RoomGUI()
