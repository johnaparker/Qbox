#include "sources/time_profile.h"

using namespace std;

namespace qbox {
    gaussian_time::gaussian_time(double f, double sig, double T0): f(f), sig(sig), T0(T0) {};

    double gaussian_time::response(double t) const {
        return sin(2*M_PI*f*t)*exp(-1*(pow((t-T0)*sig*2*M_PI,2)));
    }

    void gaussian_time::write(const h5cpp::h5group &group) const {
        auto dspace = h5cpp::dspace(std::vector<hsize_t>{1});
        auto dset = group.create_dataset("frequency", h5cpp::dtype::Double, dspace);
        dset.write(&f);
        dset = group.create_dataset("T0", h5cpp::dtype::Double, dspace);
        dset.write(&T0);
        dset = group.create_dataset("sig", h5cpp::dtype::Double, dspace);
        dset.write(&sig);
    }

    unique_ptr<time_profile> gaussian_time::clone() const {
        return unique_ptr<time_profile>(new gaussian_time(*this));
    }

    continuous_time::continuous_time(double f): f(f) {};

    double continuous_time::response(double t) const {
        return sin(2*M_PI*f*t);
    }

    void continuous_time::write(const h5cpp::h5group &group) const {
        auto dspace = h5cpp::dspace(std::vector<hsize_t>{1});
        auto dset = group.create_dataset("frequency", h5cpp::dtype::Double, dspace);
        dset.write(&f);
    }

    unique_ptr<time_profile> continuous_time::clone() const {
        return unique_ptr<time_profile>(new continuous_time(*this));
    }
}
