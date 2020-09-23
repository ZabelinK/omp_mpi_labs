#!/bin/python
import sys
import os
import random
import time 

current_milli_time = lambda: int(round(time.time() * 1000))

if len(sys.argv) != 2:
    print("Invalid count of args")
    exit(1)

N = int(sys.argv[1])

f = open('./payload', 'w')
for i in range(N):
    f.write(str(random.randint(1, 1000000)) + " ")

t1 = current_milli_time()
os.system("./sequentially " + str(N) + " < payload > /dev/null")
print("Sequentially : {} ms".format(current_milli_time() - t1))

t2 = current_milli_time()
os.system("./parallel " + str(N) + " < payload > /dev/null")
print("Parallel : {} ms".format(current_milli_time() - t2))
