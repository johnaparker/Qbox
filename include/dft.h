#ifndef GUARD_dft_h
#define GUARD_dft_h

#include "h5cpp.h"
#include "vec.h"
#include <array>
#include <vector>
#include <complex>
#include <initializer_list>
#include <functional>

namespace qbox {

    using namespace std::complex_literals;

    template<unsigned int RANK>
    struct complex_dft_tensor {
        using dft_tensor = Eigen::Tensor<double, RANK+1, Eigen::RowMajor>;
        complex_dft_tensor(const dft_tensor &real, const dft_tensor &imag): real(real), imag(imag) {};
        dft_tensor real, imag;

        void write(const h5cpp::h5group& group, const std::string &name) const;
            // h5cpp::write_tensor(real, group, 'r' + name);
            // h5cpp::write_tensor(imag, group, 'i' + name);
    };

    namespace DFT {
        class Ez{};
        class Hx{};
        class Hy{};
        class tangent{};
        class all{};
    }

    template<unsigned int RANK>
    class dft {
        using dft_tensor  = Eigen::Tensor<double, RANK+1, Eigen::RowMajor>;
        using dft_dimensions = typename dft_tensor::Dimensions;

    public:
        dft() = default;

        dft(const Array &freq): t(0), freq(freq) {};

        dft(const Array &freq, const std::string &name, const std::array<int,RANK> &dim): dft(freq) {
            add(name, dim);
        }

        dft(const Array &freq, const std::vector<std::string> &names, const std::array<int,RANK> &dim): dft(freq) {
            add(names, dim);
        }

        void add(const std::string &name, const std::array<int,RANK> &dim) {
            // assert that name doesn't exist already
            // assert that len(dim) = RANK

            dft_dimensions dft_dims;
            for (int i = 0; i < RANK; i++)
                dft_dims[i] = dim[i];
            dft_dims[RANK] = Nfreq();

            fourier.insert({name, complex_dft_tensor<RANK>(dft_tensor(dft_dims), dft_tensor(dft_dims))});
            fourier.at(name).real.setZero();
            fourier.at(name).imag.setZero();
        }

        void add(const std::vector<std::string> &names, const std::array<int,RANK> &dim) {
            for (const auto&& name : names)
                add(name, dim);
        }

        template <class T>
        void update(std::function<std::function<T>(std::string)> f, double tnew);


        void write_properties(const h5cpp::h5group &group) const {
            h5cpp::write_array<double>(freq, group, "dft_frequency");
        }

        void write(const h5cpp::h5group &group) const {
            //write fourier
            for (const auto& p: fourier)
                p.second.write(group, p.first);
        }

        //getter functions
        Array get_freq() const {return freq;}
        double get_freq(int i) const {return freq[i];}
        int Nfreq() const {return freq.size();}

        const complex_dft_tensor<RANK>& operator() (const std::string &name) const {return fourier.at(name);}
        const complex_dft_tensor<RANK>& get(const std::string &name) const {return fourier.at(name);}

        void operator-=(const dft& other) {
            // subtract fourier's only if the following are equal: RANK, freq, fourier's keys
        }

    private:
        double t;     ///< current time
        Array freq;   ///< frequency data

        std::map<std::string, complex_dft_tensor<RANK>> fourier;
    };
}

#endif