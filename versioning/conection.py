import ctypes

clibrary = ctypes.CDLL("C:/Users/tomje/OneDrive/EPFL/BA3/CMT/Pro-cul-feur/connection.so")

#clibrary.display(b"lol", 22) #b is for binary (only way to pass string)


func = clibrary.display

func.argtypes = [ctypes.c_char_p, ctypes.c_int] #declares the argument type
func.restype =  ctypes.c_char_p #returns a char

func(b"lol", 22)