import h5py
import numpy as np
import matplotlib.pyplot as plt

with h5py.File('out.h5', 'r') as f:
    plt.figure(1)

    for name in ['left', 'right', 'bottom', 'top', 'center']:
        data = f['fields/{}/Ez'.format(name)][...][::100]
        data /= np.max(data[:100])
        plt.plot(data[:], label = name)
    plt.legend()
    plt.ylim([-8, 8])

    plt.figure(2)
    data = f['fields/{}/Ez'.format("left_line")][:, 100*100]
    plt.plot(data)

    plt.figure(3)
    data = f['fields/{}/Ez'.format("top_line")][:, 100*100]
    plt.plot(data)
    
    plt.show()
