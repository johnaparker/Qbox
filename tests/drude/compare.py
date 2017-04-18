import numpy as np
import matplotlib.pyplot as plt
import h5py

freq = np.linspace(1/30, 3/30, 200)
wav = 1/freq

with h5py.File("scat.h5", 'r') as f:
    inc = f["sources/tfsf/flux"][...]
    scat = f["monitors/cylinder_monitor/box_scat/flux"][...]
    # scat = f["monitors/box_monitor/box_scat/flux"][...]

scat_qbox = scat/inc
plt.plot(wav, scat_qbox, label = "qbox")

theory = np.loadtxt("mie.out").T
plt.plot(theory[0], theory[1], label="exact")

plt.legend()

plt.show()
