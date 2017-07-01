import h5py
import numpy as np
import matplotlib.pyplot as plt
import my_pytools.my_matplotlib.colors as colors 
import matplotlib.animation as animation


filename = "metal_sphere"
filename = "metal_ellipse"
filename = "dielectric_block"
filename = "metal_block"
filename = "out"

with h5py.File('{}.h5'.format(filename), 'r') as f:
    d = f["monitors/volume_monitor/monitor_0/Ez"][...]
    Er = d['real']
    Ei = d['imag']
    I = Er**2 + Ei**2
    I = I.squeeze()
    vmax = np.max(I)
    vmin = 0
    plt.gca().set_aspect('equal')
    im = plt.pcolormesh(I.T, cmap=colors.cmap['parula'], vmax = vmax, vmin = vmin)
    
    def updatefig(frame):
        d = f["monitors/volume_monitor/monitor_{0}/Ez".format(frame)][...]
        Er = d['real']
        Ei = d['imag']
        I = Er**2 + Ei**2
        I = I.squeeze()
        im.set_array(np.ravel(I.T))

        return im,

    writer = animation.FFMpegWriter(fps=30, bitrate=4000)
    ani = animation.FuncAnimation(plt.gcf(), updatefig, np.arange(0,100), interval=30, blit=False)
    # ani.save("{}.mp4".format(filename), writer=writer)
    plt.show()
