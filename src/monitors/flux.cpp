#include "monitors/flux.h"

namespace qbox {

    Flux::Flux(const Array& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
        file_name = file.get_name();
        group_path = group.get_path();
    }

    Flux::Flux(const Array& S, const std::string &file_name, const std::string &group_path): S(S), file_name(file_name), group_path(group_path) {};

    void Flux::write() {
        h5cpp::h5file f(file_name, h5cpp::io::rw);
        auto g = f.open_group(group_path);
        h5cpp::write_array<double>(S, g, "flux");
    }


    Array compute_flux(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {
        const int length = E.real.dimensions()[0];
        const int Nfreq = E.real.dimensions()[1];

        Array S = Array::Zero(Nfreq);
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < Nfreq; j++) {
                S[j] += E.real(i,j)*H.real(i,j) + E.imag(i,j)*H.imag(i,j);
            }
        }
        
        S *= da*int(Sign);
        return S;
    }

}
