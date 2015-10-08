#ifndef GUARD_matrix_h
#define GUARD_matrix_h

template <class T>
class matrix {
public:
    T *data;
    int Nx,Ny;
public:
    matrix() = default;
    matrix(const matrix&) = default;
    matrix& operator=(const matrix&) = default;
    matrix(T* data,int Nx,int Ny): data(data), Nx(Nx), Ny(Ny) {};
    T* operator[](int index) {return data+index*Ny;}
};

#endif
