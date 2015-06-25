#include "matrix.h"

matrix::matrix(double *data, int Nx, int Ny): data(data), Nx(Nx), Ny(Ny) {};
double* matrix::operator[] (int index) {
    return data+index*Ny;
}
