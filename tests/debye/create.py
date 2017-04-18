import numpy as np
filename = "eps.txt"

freq = np.linspace(1/30, 3/30, 200)
omega = 2*np.pi*freq
wav = 1/freq
eps_inf = 2
delta = 1
tau = 0.2

eps = eps_inf + delta/(1 + 1j*omega*tau)

to_file = np.array([wav[::-1], eps.real[::-1], np.abs(eps.imag[::-1])])
np.savetxt(filename, to_file.T)
