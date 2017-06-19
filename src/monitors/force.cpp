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

    tensor compute_force(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {
    }

    tensor compute_torque(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign) {
    }

}
