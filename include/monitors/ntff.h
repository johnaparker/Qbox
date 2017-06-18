#ifndef GUARD_ntff_h
#define GUARD_ntff_h

#include <string>
#include "h5cpp.h"
#include "../vec.h"
#include "../dft.h"

namespace qbox {

    class ntff_point {
    public:
        ntff_point(const ComplexArray& S, const h5cpp::h5file &file, const h5cpp::h5group &group );
        ntff_point(const ComplexArray& S, const std::string &file_name, const std::string &group_path);

        void write();
        ComplexArray data() {return S;}

    private:
        ComplexArray S;
        std::string file_name;
        std::string group_path;

        //write as attrs:
        //pos
        //center
    };

    class ntff_sphere {
    public:
        ntff_sphere(const ComplexTensor &S, const h5cpp::h5file &file, const h5cpp::h5group &group );
        ntff_sphere(const ComplexTensor &S, const std::string &file_name, const std::string &group_path);
        void write();

        ComplexTensor data() {return S;}

    private:
        ComplexTensor S;
        std::string file_name;
        std::string group_path;

        //write as attrs:
        //center
        //radius
    };

    ComplexArray compute_ntff_point(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign);

    ComplexTensor compute_ntff_sphere(const complex_dft_tensor<1> &E, const complex_dft_tensor<1> &H, double da, sign Sign);

}

#endif
