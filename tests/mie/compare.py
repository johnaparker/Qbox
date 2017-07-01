import numpy as np
import matplotlib.pyplot as plt
import h5py

freq = np.linspace(1/30, 3/30, 200)
wav = 1/freq

def L2_norm(y1,y2):
    return np.linalg.norm(y1-y2)

def sup_norm(y1,y2):
    i = np.argmax(np.abs(y1-y2))
    return np.abs(y1-y2)[i], wav[-i]

with h5py.File("scat.h5", 'r') as f:
    inc = f["sources/tfsf/flux"][...]
    scat = f["monitors/cylinder_monitor/scat/flux"][...]
    # scat = f["monitors/box_monitor/box_scat/flux"][...]

scat_qbox = scat/inc
plt.plot(wav, scat_qbox, label = "qbox")

plt.figure(1)
with h5py.File('../../bench/test2.h5', 'r') as f:
    inc = f['inc'][...]
    scat = f['box_scat'][...]

    scat_meep = scat/inc*40
    plt.plot(wav,scat_meep, label="meep")

theory = np.loadtxt("../../bench/mie-theory-cylinder/sig.mie.dat").T
plt.plot(theory[0], theory[1], label="exact")

print("L2 norm (meep): {0:.2f}".format(L2_norm(theory[1], scat_meep[::-1])))
print("sup norm (meep): {0:.2f} at {1:.2f}".format(*sup_norm(theory[1], scat_meep[::-1])))

print("L2 norm (qbox): {0:.2f}".format(L2_norm(theory[1], scat_qbox[::-1])))
print("sup norm (qbox): {0:.2f} at {1:.2f}".format(*sup_norm(theory[1], scat_qbox[::-1])))

plt.legend()

plt.show()
