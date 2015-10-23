#ifndef GUARD_matrix_h
#define GUARD_matrix_h

//this should be benchmarked

template <class T>
class matrix {
public:
    T *mData;
    int Nx,Ny;
public:
    matrix() = default;
    matrix(const matrix&) = default;
    matrix& operator=(const matrix&) = default;
    matrix(T* data,int Nx,int Ny): mData(data), Nx(Nx), Ny(Ny) {};
    T* operator[](int index) {return mData+index*Ny;}
    T* data() {
        return mData;
    }
    int get_Nx() {
        return Nx;
    }
    int get_Ny() {
        return Ny;
    }
};

#endif
