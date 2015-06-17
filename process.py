#!/usr/bin/python
import pylab as py
import numpy as np
import matplotlib.animation as animation
import sys
from copy import copy

class field:
    def __init__(self,sx):
        self.sx = sx
        self.E = []
        self.H = []

    def load(self,Efile,Hfile):
        Ef = open(Efile)
        Hf = open(Hfile)
        x = 0
        Evals = np.zeros(sx)
        Hvals = np.zeros(sx)
        for e,h in zip(Ef,Hf):
            Evals[x] = float(e)
            Hvals[x] = float(h)
            x += 1
            if x == self.sx:
                x = 0
                self.E.append(copy(Evals))
                self.H.append(copy(Hvals))

Efile = "Eout.txt"
Hfile = "Hout.txt"

argc = len(sys.argv)
if argc == 2:
    sx = int(sys.argv[1])
else:
    raise Exception("Incorrect number of arguments")
    
    
F = field(sx)
F.load(Efile,Hfile)
st = len(F.E)

fig,ax = py.subplots()
line, = ax.plot(F.E[0])
line2, = ax.plot(F.H[0])
py.xlim([0,sx])
py.ylim([-1,1])
py.axvline(x=sx/2,color='r',linewidth=2)
def animate(i):
    line.set_ydata(F.E[i])
#    line2.set_ydata(F.H[i])
    return line,line2

def init():
    line.set_ydata(np.ma.array(np.arange(len(F.E[0])), mask=True))
    return line,

ani = animation.FuncAnimation(fig,animate,np.arange(1,st),init_func=init,interval=15,blit=True)
py.show()
