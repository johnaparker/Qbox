import numpy as np
import matplotlib.pyplot as plt
import h5py as h5
from scipy.optimize import curve_fit

def load_flux(filename, dataname):
    h5file = h5.File(filename, 'r')
    h5data = h5file[dataname]
    flux = np.array(h5data)
    return flux

def gaussian(x, A, x0, sig, c):
    return A*np.exp(-(x-x0)**2/(2*sig**2)) + c



if __name__ == "__main__":
    f = load_flux("../build/out.h5", "m1")
    x = np.linspace(1/30,3/30,len(f))
    plt.plot(x, -f, '.')

    res,var = curve_fit(gaussian, x, -f, (1,1,1,0)) 
    y_fit = gaussian(x, *res)
    plt.plot(x,y_fit, linewidth=2, color='r')

    print("A: {}".format(res[0]))
    print("f0: {}".format( res[1]))
    print("df: {}".format(abs( res[2])))
    print("c: {}".format(abs( res[3])))
    print("")
    print("l0: {}".format( 1/res[1]))
    print("dl: {}".format(abs( 1/res[2])))

    plt.show()
