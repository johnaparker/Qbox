#include "dft.h"

namespace qbox {

    //template specialization needed because Eigen can't cast when RANK is a template parameter for reasons I know not
    template<>
    void complex_dft_tensor<0>::write(const h5cpp::h5group& group, const std::string &name) const {
        Eigen::Tensor<std::complex<double>, 1, Eigen::RowMajor> combined = 
                real.cast<std::complex<double>>() + 1i*imag.cast<std::complex<double>>();
        h5cpp::write_tensor(combined, group, name);
    }

    template<>
    void complex_dft_tensor<1>::write(const h5cpp::h5group& group, const std::string &name) const {
        Eigen::Tensor<std::complex<double>, 2, Eigen::RowMajor> combined = 
                real.cast<std::complex<double>>() + 1i*imag.cast<std::complex<double>>();
        h5cpp::write_tensor(combined, group, name);
    }

    template<>
    void complex_dft_tensor<2>::write(const h5cpp::h5group& group, const std::string &name) const {
        Eigen::Tensor<std::complex<double>, 3, Eigen::RowMajor> combined = 
                real.cast<std::complex<double>>() + 1i*imag.cast<std::complex<double>>();
        h5cpp::write_tensor(combined, group, name);
    }

    template<>
    template<> void dft<0>::update(std::function<std::function<double()>(std::string)> f, double tnew) {
        if (t == tnew)
            return;
        t = tnew;
        // assert that keys match exactly

        Array cosf = Eigen::cos(2*M_PI*freq*t);
        Array sinf = Eigen::sin(2*M_PI*freq*t);

        const int kmax = Nfreq();
        for (auto& p: fourier) {
            auto g = f(p.first);
            double g_val = g();
            for (int k = 0; k < kmax; k++) {
                p.second.real(k) += g_val*cosf(k);
                p.second.imag(k) += g_val*sinf(k);
            }
        }
    }


    template<>
    template<> void dft<1>::update(std::function<std::function<double(int)>(std::string)> f, double tnew) {
        if (t == tnew)
            return;
        t = tnew;
        // assert that keys match exactly

        Array cosf = Eigen::cos(2*M_PI*freq*t);
        Array sinf = Eigen::sin(2*M_PI*freq*t);

        const int kmax = Nfreq();
        for (auto& p: fourier) {
            auto g = f(p.first);
            const int imax = p.second.real.dimensions()[0];

            for (int i = 0; i < imax; i++ ) {
                double g_val = g(i);
                for (int k = 0; k < kmax; k++) {
                    p.second.real(i,k) += g_val*cosf(k);
                    p.second.imag(i,k) += g_val*sinf(k);
                }
            }
        }
    }

    template<>
    template<> void dft<2>::update(std::function<std::function<double(int,int)>(std::string)> f, double tnew) {
        if (t == tnew)
            return;
        t = tnew;
        // assert that keys match exactly

        Array cosf = Eigen::cos(2*M_PI*freq*t);
        Array sinf = Eigen::sin(2*M_PI*freq*t);

        const int kmax = Nfreq();
        for (auto& p: fourier) {
            auto g = f(p.first);
            const int imax = p.second.real.dimensions()[0];
            const int jmax = p.second.real.dimensions()[1];

            for (int i = 0; i < imax; i++ ) {
                for (int j = 0; j < jmax; j++ ) {
                    double g_val = g(i,j);
                    for (int k = 0; k < kmax; k++) {
                        p.second.real(i,j,k) += g_val*cosf(k);
                        p.second.imag(i,j,k) += g_val*sinf(k);
                    }
                }
            }
        }
    }
}