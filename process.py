#!/usr/bin/python
import pylab as py
import numpy as np
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from multiprocessing import Pool,Process,Value
import sys
from copy import copy

class field:
    def __init__(self,sx,sy):
        self.sx = sx
        self.sy = sy
        self.E = []
        self.H = []

    def load(self,Efile,Hfile):
        Ef = open(Efile, 'rb')
        #Hf = open(Hfile, 'rb')
        E = np.fromfile(Ef)
        #H = np.fromfile(Hf)

        size = self.sx*self.sy
        tSteps = int(np.floor(len(E)/size))
        for t in range(tSteps):
            E_t = np.reshape(E[t*size:(t+1)*size],(sx,sy))
            self.E.append(E_t)
            #self.H.append(H[t*self.sx:(t+1)*self.sx])

Efile = "Eout.dat"
Hfile = "Hout.dat"

argc = len(sys.argv)
if argc == 3:
    sx = int(sys.argv[1])
    sy = int(sys.argv[2])
else:
    raise Exception("Incorrect number of arguments")
    
F = field(sx,sy)
F.load(Efile,Hfile)

x = np.arange(sx)
y = np.arange(sy)
x,y = np.meshgrid(x,y)
z = x*0

def f(a,b,c):
    for i in range(a,b):
        print(c.value)
        fig = py.figure(i)
        ax = fig.gca(projection='3d')
        z = F.E[i]
        surf = ax.plot_surface(x,y,z,rstride=1,cstride=1)
        py.savefig(r'./figs/fig' + str(i+1).zfill(3) + '.png')
        py.close(i)
        c.value += 1

proc = []
cores = 4
steps = len(F.E) 
size = int(np.floor(steps/cores))
c = Value('i', 0)

for i in range(cores):
    p = Process(target=f, args=(i*size, (i+1)*size, c))
    p.start()
    proc.append(p)

for p in proc:
    p.join()

