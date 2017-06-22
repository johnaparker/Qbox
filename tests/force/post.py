import numpy as np
import matplotlib.pyplot as plt
import h5py

freq = np.linspace(1/30, 3/30, 200)
wav = 1/freq

with h5py.File("../../build/scat.h5", 'r') as f:
    inc = f["sources/tfsf/flux"][...]
    F = f["monitors/box_monitor/monitor_0/force"][...]
    T = f["monitors/box_monitor/monitor_0/torque"][...]

    plt.plot(wav, F[0,:]/inc, label = 'Fx, qbox')
    plt.plot(wav, F[1,:]/inc, label = 'Fy, qbox')
    plt.plot(wav,T/inc/40, label = 'Tz, qbox')

with h5py.File('../../bench/force.h5', 'r') as f:
    inc = f['inc'][...]
    Fx = f["Fx"][...]
    Fy = f["Fy"][...]

    plt.plot(wav, Fx/inc, label = 'Fx, meep')
    plt.plot(wav, Fy/inc*40, label = 'Fy, meep')

plt.legend()
plt.show()
