import h5py
import numpy as np
import matplotlib.pyplot as plt
import my_pytools.my_matplotlib.colors as colors 


with h5py.File('out.h5', 'r') as f:
    d = f["monitors/volume_monitor/vol/dft"][...]
    Er = d['real']
    Ei = d['imag']
    I = Er**2 + Ei**2
    I = I.squeeze()
    phase = np.arctan2(Ei,Er).squeeze()
    

plt.gca().set_aspect('equal')
plt.pcolormesh(I.T, cmap=colors.cmap['parula'])
# plt.pcolormesh(phase.T, cmap = colors.cmap['phase'])
plt.show()
