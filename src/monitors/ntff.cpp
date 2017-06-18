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

    ComplexArray compute_ntff_point(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {

    }

    ComplexTensor compute_ntff_sphere(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {

    }

}
