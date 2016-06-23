#ifndef GUARD_matrix_h
#define GUARD_matrix_h

#include <iostream>
#include <algorithm>
#include <iostream>
#include <memory>

namespace qbox {

/// Matrix class for storing 2D data
/**
 *  Matrix (template) creates an Nx by Ny matrix of type T on the heap.
 *  Data can be accessed using M[x][y]
 *  @todo Matrix should be benchmarked against other methods
 */
    template <class T>
    class matrix {
    public:
        matrix() {
            mData = nullptr;
            Nx = 0;
            Ny = 0;
        }

        matrix(matrix&&) = default;
        matrix& operator=(matrix&&) = default;

        /// construct a Nx by Ny matrix
        matrix(int Nx,int Ny): Nx(Nx), Ny(Ny) {
            mData = std::make_unique<T[]>(Nx*Ny);
            //for (int i = 0; i != Nx; i++) {
                //for (int j = 0; j != Ny; j++)
                    //(*this)[i][j] = 0;
            //}
        };

        /// get reference to row i
        /**
         * &param index row number
         * &return row array pointer of size Ny
         */
        T* operator[](int index) const {return mData.get()+index*Ny;}

        /// get value at row i, col j
        T get(int i, int j) const {return mData.get()[i*Ny+j];}

        /// get pointer to data
        T* data() const {
            return mData.get();
        }
        /// get Nx
        int get_Nx() const {
            return Nx;
        }
        /// get Ny
        int get_Ny() const {
            return Ny;
        }

    private:
        std::unique_ptr<T[]> mData; ///<  Pointer to data
        int Nx,Ny;  ///< Size of the matrix
    };
}

#endif
