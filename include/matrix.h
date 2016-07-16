#ifndef GUARD_matrix_h
#define GUARD_matrix_h

#include <iostream>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <cstdarg>
#include <algorithm>
#include <numeric>

namespace qbox {

/// Matrix class for storing 2D data
/**
 *  Matrix (template) creates an Nx by Ny matrix of type T on the heap.
 *  Data can be accessed using M[x][y]
 *  @todo Matrix should be benchmarked against other methods
 */
    template <class T, int D>
    class matrix {
    public:
        matrix() {
            mData = nullptr;
            dims.resize(D);
        }

        matrix(matrix&&) = default;
        matrix& operator=(matrix&&) = default;

        /// construct a Nx by Ny matrix
        matrix(std::initializer_list<int> init_dims): dims(init_dims) {
            mData = std::make_unique<T[]>(tot());

            offsets.push_back(1);
            for (int i = 1; i != D; i++)
                offsets.push_back( offsets[i-1]*dims[D-i] );
            reverse(offsets.begin(), offsets.end());
        };

        /// get reference to row i
        /**
         * &param index row number
         * &return row array pointer of size Ny
         */
        T& operator() (const std::initializer_list<int>& i) {
            int index = std::inner_product(i.begin(), i.end(), offsets.begin(), 0);
            return *(mData.get() + index);
        }

        const T& operator() (const std::initializer_list<int>& i) const {
            int index = std::inner_product(i.begin(), i.end(), offsets.begin(), 0);
            return *(mData.get() + index);
        }

        /// get value at row i, col j
        T get(const std::initializer_list<int>& i) const {
            int index = std::inner_product(i.begin(), i.end(), offsets.begin(), 0);
            return *(mData.get() + index);
        }

        /// get pointer to data
        T* data() const {
            return mData.get();
        }

        int tot() {
            int ret = 1;
            for (const auto x: dims)
                ret *= x;
            return ret;
        }

    private:
        std::unique_ptr<T[]> mData; ///<  Pointer to data
        std::vector<int> dims;
        std::vector<int> offsets;
    };
}

#endif
