#ifndef GUARD_matrix_h
#define GUARD_matrix_h

class matrix {
public:
    double *data;
    int Nx,Ny;
public:
    matrix() = default;
    matrix(const matrix&) = default;
    matrix& operator=(const matrix&) = default;
    matrix(double* data,int Nx,int Ny);
    double* operator[](int index);
};

#endif
