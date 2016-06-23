import numpy as np
import matplotlib.pyplot as plt
import h5py as h5

def load_flux(filename, dataname):
    h5file = h5.File(filename, 'r')
    h5data = h5file[dataname]
    flux = np.array(h5data)
    return flux


if __name__ == "__main__":
    f = load_flux("../build/out.h5", "m1")
    plt.plot(f)
    plt.show()
