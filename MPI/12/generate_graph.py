#!/bin/python3

import argparse
import numpy as np
import os
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
from mpl_toolkits.mplot3d import Axes3D

parser = argparse.ArgumentParser(description="Generate and map data for matrix vector multeplexion")
parser.add_argument('--m_begin', type=int, default=100, help="m start")
parser.add_argument('--m_end', type=int, default=1000, help="m end")
parser.add_argument('--m_step', type=int, default=100, help="m step")

parser.add_argument('--n_begin', type=int, default=100, help="n start")
parser.add_argument('--n_end', type=int, default=1000, help="n end")
parser.add_argument('--n_step', type=int, default=100, help="n step")

parser.add_argument('--p_begin', type=int, default=10, help="p start")
parser.add_argument('--p_end', type=int, default=100, help="p end")
parser.add_argument('--p_step', type=int, default=10, help="p step")

args = parser.parse_args()

def calculate(m, p, n):
    print("!!! ", m, p, n)
    os.system('./generator.py -m {} -n {} -p {}'.format(m, n, p))
    with open('./time', 'r') as f:
        time = f.read()
        return time

m_range = np.arange(args.m_begin, args.m_end, args.m_step)
n_range = np.arange(args.n_begin, args.n_end, args.n_step)
p_range = np.arange(args.p_begin, args.p_end, args.p_step)

X, Y = np.meshgrid(m_range, p_range)



results = np.zeros(( len(m_range), len(p_range)))
n = 200
m_iter = 0
p_iter = 0

os.system('./generator.py -m 100 -n 100 -p 10')

for m in m_range:
    p_iter = 0
    for p in p_range:
        os.system('./generator.py -m {} -n {} -p {}'.format(m, n, p))
        with open('./time', 'r') as f:
            time = f.read()
            results[m_iter, p_iter] = time
        p_iter += 1
    m_iter += 1

fig = plt.figure()
ax = fig.gca(projection='3d')

# Plot the surface.
surf = ax.plot_surface(X, Y, results, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

# Customize the z axis.
#ax.set_zlim(-1.01, 1.01)
ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))
ax.set_ylabel("Number of process")
ax.set_xlabel("M size (matrix dimetion)")
ax.set_zlabel("Time")

# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)


plt.show()