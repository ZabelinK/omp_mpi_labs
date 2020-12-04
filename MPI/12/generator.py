#!/bin/python3

import argparse
import numpy as np
import random
import os
import logging
import time

parser = argparse.ArgumentParser(description="Generate and map data for matrix vector multeplexion")
parser.add_argument('-m', type=int, required=True, help="Number of matrix rows")
parser.add_argument('-n', type=int, required=True, help="Number of matrix columns and vector size")
parser.add_argument('-p', type=int, required=True, help="Number of processes which will process data")
parser.add_argument('--min', type=int, default=-1000, help="Minimal generated number")
parser.add_argument('--max', type=int, default=1000, help="Maximum generated number")
args = parser.parse_args()

logger = logging.Logger("MPI Logger")
logger.setLevel(logging.DEBUG)

logger.warning("Generate matrix ({}, {}) for {} processes".format(args.m, args.n, args.p))

matrix = np.random.randint(args.min, args.max, (args.m, args.n))
vector = np.random.randint(args.min, args.max, (args.n, 1))
result = np.matmul(matrix, vector)

try:
    logger.warning("Creating output directories")
    os.system('mkdir ./data')
    os.system('mkdir ./data/mapped_data')
except:
    pass

logger.warning("Saving data...")
np.savetxt('./data/matrix.txt', matrix, fmt="%i")
np.savetxt('./data/vector.txt', vector, fmt='%i')
np.savetxt('./data/np_result.txt', result, fmt='%i')

row_iter = 0
max_row_per_process = (args.m // (args.p)) + 1

for n in range(args.p):
    with open('./data/mapped_data/{}.txt'.format(n), "w") as f:
        number_of_rows = min(args.m - row_iter, max_row_per_process)
        f.write("{} {}\n".format(number_of_rows, row_iter))

        for i in range(number_of_rows):
            f.write(' '.join(map(str, matrix[row_iter,:])) + "\n")
            row_iter += 1

logger.warning("Calling MPI application to multeplex matrix and vector")
start_time = time.time()
os.system("mpirun -n {} ./main {} {}".format(args.p, args.m, args.n))
res_time = time.time() - start_time
logger.warning("----- Time : {} ------".format(res_time))

mpi_result = np.loadtxt('./data/result.txt', ndmin=2)
if (result != mpi_result).all():
    logger.error("MPI result isn't equal to NumPy result. There is a bug")

with open('./time', "w") as f:
    f.write(str(res_time))