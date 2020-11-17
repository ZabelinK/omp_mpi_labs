#!/bin/python
import sys
import random

N = int(sys.argv[1])

f = open('./text.txt', 'w')
f.write(str(N))

for i in range(N * N):
    if i % N == 0:
        f.write("\n")
    f.write(str(random.randint(-100000, 100000)))
    f.write(" ")



f.close()