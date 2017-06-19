#include "monitors/ntff.h"

namespace qbox {

    ntff_point::ntff_point(const ComplexArray& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
        file_name = file.get_name();
        group_path = group.get_path();
    }

    ntff_point::ntff_point(const ComplexArray& S, const std::string &file_name, const std::string &group_path): S(S), file_name(file_name), group_path(group_path) {};

    void ntff_point::write() {
        h5cpp::h5file f(file_name, h5cpp::io::rw);
        auto g = f.open_group(group_path);
        h5cpp::write_array<std::complex<double>>(S, g, "ntff_point");
    }

    ntff_sphere::ntff_sphere(const ComplexTensor& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
        file_name = file.get_name();
        group_path = group.get_path();
    }

    ntff_sphere::ntff_sphere(const ComplexTensor& S, const std::string &file_name, const std::string &group_path): S(S), file_name(file_name), group_path(group_path) {};

    void ntff_sphere::write() {
        h5cpp::h5file f(file_name, h5cpp::io::rw);
        auto g = f.open_group(group_path);
        h5cpp::write_tensor(S, g, "ntff_sphere");
    }

    ComplexArray compute_ntff_point(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, const Array &freq, const vec &center, const vec &pc) {
        const int Nfreq = freq.size();

        vec p = pc - center;
        double r = p.norm();
        Array omega = 2*M_PI*freq;
        Array k = omega;
        ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
        ComplexArray integral = ComplexArray::Zero(Nfreq);
        
        //*** find a way to write this generically
        // vec tangent = surf.tangent;
        // vec normal = surf.normal.array().cwiseAbs();  //*** this needs to be outward normal

        // double Jsgn = tangent.dot(vec(1,1));
        // double Msgn = tangent.dot(vec(-1,1));

        // const auto E = fourier("Ez");
        // const auto H = surf.dim[0] == 0 ? fourier("Hy") : fourier("Hx");


        //     for (int i = 0; i < length; i++) {
        //         vec p_prime = surf.a + tangent*i*F->dx - center; //*** account for yee grid half-step here
        //         double angle = p.dot(normal)/r;  //*** worry about the sign of normal here
        //         for (int j = 0; j < Nfreq; j++) {
        //             auto Ec = E.real(i,j) + 1i*E.imag(i,j);
        //             auto Hc = H.real(i,j) + 1i*H.imag(i,j);

        //             auto Jeq_term = Jsgn*omega[j]*Hc;
        //             auto Meq_term = Msgn*k[j]*Ec*angle;
        //             auto integand = (Jeq_term + Meq_term)*exp(1i*k[j]*p.dot(p_prime)/r);
        //             integral[j] += integand; 
        //         }
        //     }

        return factor*integral;
    }

    ComplexTensor compute_ntff_sphere(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {

    }

}
