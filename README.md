# Qbox
![](https://github.com/johnaparker/qbox/blob/master/img/dynamics.gif)

## Install
Install the required dependencies
- [HDF5](https://www.hdfgroup.org/solutions/hdf5/)
- [h5cpp](https://github.com/johnaparker/h5cpp)
- [eigen](https://eigen.tuxfamily.org/)

Install QBox using cmake
```
mkdir build && cd build
cmake ..
make
make install
```

## Examples
Example scripts in the tests folder can be compiled, e.g.
```
g++ -O3 main.cpp -lhdf5 -lh5cpp -lqbox
```


## Python post-processing library and command-line utility
[QPost](https://github.com/johnaparker/qpost) is a Python library for processing the output of Qbox.

For example, to create an animation of the z-component of the electric field
```
qbox video out.h5 fields/Ez
```

Other options provided by the CLI
```
usage: qbox [-h] {flux,video,layout,material,cross-section} ...

positional arguments:
  {flux,video,layout,material,cross-section}
                        sub-command help
    flux                flux help
    video               video help
    layout              layout help
    material            material help
    cross-section       cross-section help

optional arguments:
  -h, --help            show this help message and exit
```

## License
Qbox is licensed under the terms of the MIT license.
