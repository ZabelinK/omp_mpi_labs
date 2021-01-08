# ctypes_test.py
import ctypes
import pathlib
import numpy as np
from numba import njit
import time

if __name__ == "__main__":
    # Load the shared library into ctypes
    libname = pathlib.Path().absolute() / "./lib/libMyMath.so"
    c_lib = ctypes.CDLL(libname)

    ND_POINTER_1 = np.ctypeslib.ndpointer(dtype=np.int32, 
                                      ndim=2,
                                      flags="C")

    c_lib.matrix_mul.argtypes = [ND_POINTER_1, ND_POINTER_1, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint]
    c_lib.matrix_mul.restype = ctypes.c_void_p

    c_lib.free_matrix.argtypes = [ctypes.c_void_p]


    L = 200
    M = 500
    N = 300

    m1 = np.random.randint(size=(L, M), low=-2, high=2, dtype=np.int32)
    m2 = np.random.randint(size=(M, N), low=-2, high=2, dtype=np.int32)


    millis = int(round(time.time() * 1000))
    data_pointer = c_lib.matrix_mul(m1, m2, L, M, N)
    res = int(round(time.time() * 1000)) - millis
    print("My Lib result : ", res)

    millis = int(round(time.time() * 1000))
    np_arr = np.dot(m1, m2)
    res = int(round(time.time() * 1000)) - millis
    print("NumPy result : ", res)

    data_pointer = ctypes.cast(data_pointer,ctypes.POINTER(ctypes.c_int32))
    
    new_array = np.ctypeslib.as_array(data_pointer,shape=(L, N))

    if (new_array != np_arr).all():
        print("IT WORKS WRONG")

    c_lib.free_matrix(data_pointer)
