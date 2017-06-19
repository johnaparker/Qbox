#ifndef GUARD_force_h
#define GUARD_force_h

#include <string>
#include "h5cpp.h"
#include "../vec.h"
#include "../dft.h"

namespace qbox {

    class Force {
    public:
        Force(const tensor& S, const h5cpp::h5file &file, const h5cpp::h5group &group );
        Force(const tensor& S, const std::string &file_name, const std::string &group_path);

        void write();
        tensor data() {return S;}

    private:
        tensor S;  // 2 x Nfreq output tensor
        std::string file_name;
        std::string group_path;
    };

    class Torque {
    public:
        Torque(const Array& S, const h5cpp::h5file &file, const h5cpp::h5group &group );
        Torque(const Array& S, const std::string &file_name, const std::string &group_path);

        void write();
        Array data() {return S;}

    private:
        Array S;  // 2 x Nfreq output tensor
        std::string file_name;
        std::string group_path;
    };

    tensor compute_force(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign);
    tensor compute_torque(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign);
    // compute both??? (for efficiency)

}

#endif
