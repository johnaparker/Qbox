import h5py
import numpy as np
import matplotlib.pyplot as plt

fig,axes = plt.subplots(1,2, figsize=(12,6), subplot_kw=dict(projection='polar'))

with h5py.File("out.h5", 'r') as f:
    plt.subplot(axes[0])
    plt.title("box monitor")
    dset = f["monitors/box_monitor/monitor_0/ntff_sphere"][...]
    
    Er = dset['real'][:,50]
    Ei = dset['imag'][:,50]
    I = Er**2 + Ei**2
    theta = np.linspace(0, 2*np.pi, Er.shape[0])
    plt.plot(theta, I)

    dset = f["monitors/cylinder_monitor/monitor_1/ntff_sphere"][...]
    
    plt.subplot(axes[1])
    plt.title("cylinder monitor")
    Er = dset['real'][:,50]
    Ei = dset['imag'][:,50]
    I = Er**2 + Ei**2
    theta = np.linspace(0, 2*np.pi, Er.shape[0])
    plt.plot(theta, I)

    plt.show()



