/*
Matrix creates an Nx by Ny matrix of type T on the heap
Data can be accessed using M[x][y]
*/

#ifndef GUARD_matrix_h
#define GUARD_matrix_h

#include <iostream>
#include <algorithm>
#include <iostream>
#include <memory>

//this should be benchmarked

namespace qbox {

    template <class T>
    class matrix {
    public:
        matrix() {
            mData = nullptr;
            Nx = 1;
            Ny = 1;
        }

        matrix(matrix&&) = default;
        matrix& operator=(matrix&&) = default;

        matrix(int Nx,int Ny): Nx(Nx), Ny(Ny) {
            mData = std::make_unique<T[]>(Nx*Ny);
            //for (int i = 0; i != Nx; i++) {
                //for (int j = 0; j != Ny; j++)
                    //(*this)[i][j] = 0;
            //}
        };

        T* operator[](int index) {return mData.get()+index*Ny;}

        T get(int i, int j) const{return mData.get()[i*Ny+j];}

        T* data() {
            return mData.get();
        }
        int get_Nx() {
            return Nx;
        }
        int get_Ny() {
            return Ny;
        }

    private:
        std::unique_ptr<T[]> mData;
        int Nx,Ny;
    };
}

#endif
