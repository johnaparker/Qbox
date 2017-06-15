#ifndef GUARD_freq_h
#define GUARD_freq_h

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
    class dft {
        using grid_tensor = Eigen::Tensor<std::complex<double>, RANK, Eigen::RowMajor>;
        using dft_tensor  = Eigen::Tensor<std::complex<double>, RANK+1, Eigen::RowMajor>;
        using grid_dimensions = typename grid_tensor::Dimensions;
        using dft_dimensions = typename dft_tensor::Dimensions;

        class Ez{};
        class Hx{};
        class Hy{};
        class tangent{};
        class all{};

    public:
        dft() = default;

        dft(const Array &freq): t(0), freq(freq) {};

        dft(const Array &freq, std::string &name, const std::array<int,RANK> &dim): dft(freq) {
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

            fourier.insert({name, dft_tensor(dft_dims)});
            fourier[name].setZero();
        }

        void add(const std::vector<std::string> &names, const std::array<int,RANK> &dim) {
            for (const auto&& name : names)
                add(name, dim);
        }

        void update(const std::map<std::string, grid_tensor> &F, double tnew) {
            if (t == tnew)
                return;
            t = tnew;
            // assert that keys match exactly

            Array cosf = Eigen::cos(2*M_PI*freq*t);
            Array sinf = Eigen::sin(2*M_PI*freq*t);

            for (const auto& p : F) {
                std::string name = p.first;
#pragma GCC ivdep
                for (int k = 0; k < Nfreq(); k++)
                    fourier[name].chip(k, RANK) += (cosf(k) + 1i*sinf(k))*p.second;
                    // fourier[name].chip(k, RANK) = p.second*cosf(k) + 1i*p.second*sinf(k);
            }
        }

        template <class T>
        void update(std::function<std::function<T>(std::string)> f, double tnew);


        void write_properties(const h5cpp::h5group &group) const {
            h5cpp::write_array<double>(freq, group, "frequency");
        }

        void write(const h5cpp::h5group &group) const {
            //write fourier
        }

        //getter functions
        Array get_freq() const {return freq;}
        double get_freq(int i) const {return freq[i];}
        int Nfreq() const {return freq.size();}

        const dft_tensor& operator() (const std::string &name) const {return fourier.at(name);}
        const dft_tensor& get(const std::string &name) const {return fourier.at(name);}

        void operator-=(const dft& other) {
            // subtract fourier's only if the following are equal: RANK, freq, fourier's keys
        }

    private:
        double t;     ///< current time
        Array freq;   ///< frequency data

        std::map<std::string, dft_tensor> fourier;
    };


    template<>
    template<> void dft<1>::update(std::function<std::function<double(int)>(std::string)> f, double tnew) {
        if (t == tnew)
            return;
        t = tnew;
        // assert that keys match exactly
        ComplexArray expf = Eigen::exp(2i*M_PI*freq*t);

        const int kmax = Nfreq();
        for (auto& p: fourier) {
            auto g = f(p.first);
            const int imax = p.second.dimensions()[0];
            // std::cout << "kmax: " << kmax << std::endl;
            // std::cout << "imax: " << imax << std::endl;
            // tensor other;
            // other = tensor(2,2);
            // other.setZero();
            
            for (int i = 0; i < imax; i++ ) {
                double g_val = g(i);
#pragma GCC ivdep
                for (int k = 0; k < kmax; k++)
                    p.second(i,k) += g_val*expf(k);
            }
        }
    }
}

#endif