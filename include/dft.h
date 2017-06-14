#ifndef GUARD_freq_h
#define GUARD_freq_h

#include "h5cpp.h"
#include "vec.h"
#include <vector>
#include <complex>

namespace qbox {

    using namespace std::complex_literals;

    template<int RANK>
    class dft {
        using grid_tensor = Eigen::Tensor<double, RANK, Eigen::RowMajor>;
        using dft_tensor  = Eigen::Tensor<std::complex<double>, RANK+1, Eigen::RowMajor>;

        class Ez{};
        class Hx{};
        class Hy{};
        class tangent{};
        class all{};

    public:
        dft() = default;
        dft(const Array &freq, const std::vector<std::string> &names, const iArray& dimensions): t(0), freq(freq)  {
            // assert len(dimensions) = RANK (or use Array template parameter)
            iArray dft_dimensions(RANK+1);
            dft_dimensions << dimensions << size();
            for (const auto&& name : names) {
                fourier.insert({name, dft_tensor(dft_dimensions)});
                fourier[name].setZero();
            }
        }

        void update(const std::map<std::string, grid_tensor> &F, double tnew) {
            if (t == tnew)
                return;
            t = tnew;
            // assert that keys match exactly

            int Nfreq = size();

            Array cosf = Eigen::cos(2*M_PI*freq*t);
            Array sinf = Eigen::sin(2*M_PI*freq*t);

            for (const auto&& p : F) {
                std::string name = p.first;
#pragma GCC ivdep
                for (int k = 0; k < Nfreq; k++)
                    fourier[name].chip(k, RANK) = p.second*cosf(k) + 1i*p.second*sinf(k);
            }
        }

        void write_properties(const h5cpp::h5group &group) const {
            h5cpp::write_array<double>(freq, group, "frequency");
        }

        void write(const h5cpp::h5group &group) const {
            //write fourier
        }

        //getter functions
        Array get_freq() const {return freq;}
        double get_freq(int i) const {return freq[i];}
        int size() const {return freq.size();}

        void operator-=(const dft& other) {
            // subtract fourier's only if the following are equal: RANK, freq, fourier's keys
        }

    private:
        double t;     ///< current time
        Array freq;   ///< frequency data

        std::map<std::string, dft_tensor> fourier;
    };

}

#endif