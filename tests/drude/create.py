import numpy as np
import matplotlib.pyplot as plt
filename = "eps.txt"

freq = np.linspace(1/30, 3/30, 200)
omega = 2*np.pi*freq
wav = 1/freq
eps_inf = 2

omega_0 = 5/30*2*np.pi
gamma = 0.1*2*np.pi
omega_0 = 5/30.0
gamma = .1

eps = eps_inf - omega_0**2/(omega**2 - 1j*omega*gamma)

plt.plot(wav, eps.real)
plt.plot(wav, -eps.imag)

to_file = np.array([wav[::-1], eps.real[::-1], np.abs(eps.imag[::-1])])
np.savetxt(filename, to_file.T)

plt.show()
