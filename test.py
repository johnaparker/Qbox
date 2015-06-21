import numpy as np
import pylab as py
from mpl_toolkits.mplot3d import Axes3D
from multiprocessing import Pool,Process,Value

pool = Pool()

x = np.linspace(-5,5,50)
y = np.linspace(-5,5,50)
x,y = np.meshgrid(x,y)
z = x*0

def f(a,b,c):
    for i in range(a,b):
        print(c.value)
        fig = py.figure(i)
        ax = fig.gca(projection='3d')
        R = np.sqrt(x**2+y**2)
        z = np.sin(R + i/10.0)
        surf = ax.plot_surface(x,y,z,rstride=1,cstride=1)
        py.savefig(r'./figs/fig' + str(i+1).zfill(3) + '.png')
        py.close(i)
        c.value += 1

proc = []
cores = 4
steps = 100
size = int(np.floor(steps/cores))
c = Value('i', 0)

for i in range(cores):
    p = Process(target=f, args=(i*size, (i+1)*size, c))
    p.start()
    proc.append(p)

for p in proc:
    p.join()
