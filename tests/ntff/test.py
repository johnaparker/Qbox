import h5py
import numpy as np
import matplotlib.pyplot as plt

with h5py.File("out.h5", 'r') as f:
    dset = f["monitors/box_monitor/monitor_0/ntff_sphere"][...]
    
    Er = dset['real'][:,50]
    Ei = dset['imag'][:,50]
    I = Er**2 + Ei**2
    theta = np.linspace(0, 2*np.pi, Er.shape[0])
    ax = plt.subplot(111, projection='polar')
    plt.plot(theta, I)
    plt.show()



