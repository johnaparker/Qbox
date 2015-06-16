#!/usr/bin/python
import pylab as py
import numpy as np
import matplotlib.animation as animation
import sys

class field:
    def __init__(self,sx,st):
        self.sx = sx
        self.st = st
        self.E = [np.zeros(sx) for i in range(st)]
        self.H = [np.zeros(sx) for i in range(st)]

    def load(self,Efile,Hfile):
        Ef = open(Efile)
        Hf = open(Hfile)
        x = 0
        t = 0
        for e,h in zip(Ef,Hf):
            self.E[t][x] = float(e)
            self.H[t][x] = float(h)
            x += 1
            if x == self.sx:
                x = 0
                t += 1

Efile = "Eout.txt"
Hfile = "Hout.txt"

argc = len(sys.argv)
print(sys.argv)
if argc == 3:
    st = int(sys.argv[1])
    sx = int(sys.argv[2])
elif argc == 2:
    st = int(sys.argv[1])
    sx = 200
else:
    raise Exception("Incorrect number of arguments")
    
    
F = field(sx,st)
F.load(Efile,Hfile)

fig,ax = py.subplots()
line, = ax.plot(F.E[0])
line2, = ax.plot(F.H[0])
py.xlim([0,sx])
py.ylim([-1,1])
def animate(i):
    line.set_ydata(F.E[i])
    line2.set_ydata(F.H[i])
    return line,line2

def init():
    line.set_ydata(np.ma.array(np.arange(len(F.E[0])), mask=True))
    return line,

ani = animation.FuncAnimation(fig,animate,np.arange(1,st),init_func=init,interval=15,blit=True)
py.show()
