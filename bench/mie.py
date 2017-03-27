import h5py
import numpy as np
import matplotlib.pyplot as plt

with h5py.File('test2.h5', 'r') as f:
    inc = f['inc'][...]
    scat = f['box_scat'][...]

    plt.plot(scat/inc, label="scat")

plt.legend()
plt.show()
