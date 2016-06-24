import numpy as np
import matplotlib.pyplot as plt
import h5py as h5
from scipy.optimize import curve_fit

def load_flux(filename, dataname):
    h5file = h5.File(filename, 'r')
    h5data = h5file[dataname]
    flux = np.array(h5data)
    return flux

if __name__ == "__main__":
    norm = load_flux("../build/norm.h5", "m_norm")
    x = np.linspace(1/30,3/30,len(norm))

    scat = load_flux("../build/scat.h5", "m_scat")
    plt.plot(scat/norm)

    plt.show()
