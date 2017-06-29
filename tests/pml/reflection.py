import h5py
import numpy as np
import matplotlib.pyplot as plt

with h5py.File('small.h5', 'r') as f:
    A_small = f['fields/A/Ez'][...]
    B_small = f['fields/B/Ez'][...]
    
with h5py.File('large.h5', 'r') as f:
    A_large = f['fields/A/Ez'][...]
    B_large = f['fields/B/Ez'][...]

plt.figure(1)
error_A = np.abs(A_small - A_large)/np.abs(np.max(A_large))
error_B = np.abs(B_small - B_large)/np.abs(np.max(B_large))
plt.semilogy(error_A, label = 'Point A')
plt.semilogy(error_B, label = 'Point B')

plt.legend()
plt.xlabel('Time step')
plt.ylabel('Relative error')
plt.ylim([1e-6, 1e-1])

plt.figure(2)
plt.plot(B_large)
plt.show()
