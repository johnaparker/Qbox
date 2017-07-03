import numpy as np
import matplotlib.pyplot as plt
import h5py
from subprocess import call

def L2_norm(y1,y2):
    return np.linalg.norm(y1-y2)

def sup_norm(y1,y2):
    i = np.argmax(np.abs(y1-y2))
    return np.abs(y1-y2)[i], wav[-i]

freq = np.linspace(1/30, 3/30, 200)
omega = 2*np.pi*freq
wav = 1/freq

# qbox result
with h5py.File("scat.h5", 'r') as f:
    inc = f["sources/tfsf/flux"][...]
    scat = f["monitors/box_monitor/scat/flux"][...]

    omega_0 = f['materials/material_0/omega_0'][...]
    gamma = f['materials/material_0/gamma'][...]
    eps_inf = f['materials/material_0/eps_inf'][...]

scat_qbox = scat/inc
plt.plot(wav, scat_qbox, label = "qbox")

# meep result
with h5py.File("meep/test2.h5", 'r') as f:
    inc = f["inc"][...]/40
    scat = f["box_scat"][...]

scat_meep = scat/inc
plt.plot(wav, scat_meep, label = "meep")

# mie theory result
filename = "eps.txt"
eps = eps_inf - omega_0**2/(omega**2 - 1j*omega*gamma)
print(gamma.shape)
to_file = np.array([wav[::-1], eps.real[::-1], np.abs(eps.imag[::-1])])
np.savetxt(filename, to_file.T)

call(['../../bench/mie-theory-cylinder/a.out'], stdin=open('cyl.in'))

theory = np.loadtxt("mie.out").T
plt.plot(theory[0], theory[1], label="exact")

# print L2 errors
print("L2 norm (meep): {0:.2f}".format(L2_norm(theory[1], scat_meep[::-1])))
print("sup norm (meep): {0:.2f} at {1:.2f}".format(*sup_norm(theory[1], scat_meep[::-1])))

print("L2 norm (qbox): {0:.2f}".format(L2_norm(theory[1], scat_qbox[::-1])))
print("sup norm (qbox): {0:.2f} at {1:.2f}".format(*sup_norm(theory[1], scat_qbox[::-1])))


plt.legend()
plt.show()
