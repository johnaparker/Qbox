# Qbox

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

## License
Qbox is licensed under the terms of the MIT license.
