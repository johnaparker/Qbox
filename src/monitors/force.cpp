#include "monitors/force.h"

namespace qbox {

    Force::Force(const tensor& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
        file_name = file.get_name();
        group_path = group.get_path();
    }

    Force::Force(const tensor& S, const std::string &file_name, const std::string &group_path): S(S), file_name(file_name), group_path(group_path) {};

    void Force::write() {
        h5cpp::h5file f(file_name, h5cpp::io::rw);
        auto g = f.open_group(group_path);
        h5cpp::write_tensor(S, g, "force");
    }

    Torque::Torque(const Array& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
        file_name = file.get_name();
        group_path = group.get_path();
    }

    Torque::Torque(const Array& S, const std::string &file_name, const std::string &group_path): S(S), file_name(file_name), group_path(group_path) {};

    void Torque::write() {
        h5cpp::h5file f(file_name, h5cpp::io::rw);
        auto g = f.open_group(group_path);
        h5cpp::write_array<double>(S, g, "torque");
    }

    tensor compute_force(const complex_dft_tensor<1> &Ez, const complex_dft_tensor<1> &Ht,
                         const complex_dft_tensor<1> &Hn, const std::function<vec(int)>& tangent, 
                         const std::function<vec(int)>& normal, double da, sign Sign) {
        
        const int length = Ez.real.dimensions()[0];
        const int Nfreq = Ez.real.dimensions()[1];
        tensor F(2, Nfreq); F.setZero();   // F = (Ft, Fn)

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < Nfreq; j++) {
                auto Ezc = Ez.real(i,j) +  1i*Ez.imag(i,j);
                auto Htc = Ht.real(i,j) + 1i*Ht.imag(i,j);
                auto Hnc = Hn.real(i,j) + 1i*Hn.imag(i,j);
                auto Hsq = std::norm(Htc) + std::norm(Hnc);
                auto Esq = std::norm(Ezc);
                Eigen::Matrix2d sigma;
                
                sigma << std::norm(Htc) - 0.5*Hsq-0.5*Esq     , real(std::conj(Htc)*Hnc),
                            real(Htc*std::conj(Hnc)), std::norm(Hnc) - 0.5*Hsq-0.5*Esq;   // Maxwell Stress Tensor for 2D TM
                vec dF = sigma*vec(0,1)*int(Sign)*da;
                F(0,j) += dF(0)*tangent(i)(0) + dF(1)*normal(i)(0);
                F(1,j) += dF(0)*tangent(i)(1) + dF(1)*normal(i)(1);
            }
        }

        return F;
    }

    tensor compute_torque(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {
    }

}
