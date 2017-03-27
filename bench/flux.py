import h5py
import numpy as np
import matplotlib.pyplot as plt

with h5py.File('test.h5', 'r') as f:
    S1 = f['S1'][...]
    S2 = f['S2'][...]

    plt.plot(S1, label="m1")
    plt.plot(S2, label="m2")

plt.legend()
plt.show()
