#include "sources/time_profile.h"

using namespace std;

namespace qbox {

    void time_profile::update(double val, double t) {
        if (fourier) {
            function<double()> f = [val](){return val;};
            function<function<double()>(string)> g = [f](string){return f;};
            fourier->update<double()>(g, t);
        }
    }

    gaussian_time::gaussian_time(double f, double sig, double T0): f(f), sig(sig), T0(T0) {};

    double gaussian_time::response(double t) {
        double val = sin(2*M_PI*f*t)*exp(-1*(pow((t-T0)*sig*2*M_PI,2)));
        update(val, t);
        return val;
    }

    void gaussian_time::write(const h5cpp::h5group &group) const {
        h5cpp::write_scalar(f, group, "frequency");
        h5cpp::write_scalar(T0, group, "T0");
        h5cpp::write_scalar(sig, group, "sig");
    }

    unique_ptr<time_profile> gaussian_time::clone() const {
        return unique_ptr<time_profile>(new gaussian_time(*this));
    }

    continuous_time::continuous_time(double f): f(f) {};

    double continuous_time::response(double t) {
        double val = sin(2*M_PI*f*t);
        update(val, t);
        return val;
    }

    void continuous_time::write(const h5cpp::h5group &group) const {
        h5cpp::write_scalar(f, group, "frequency");
    }

    unique_ptr<time_profile> continuous_time::clone() const {
        return unique_ptr<time_profile>(new continuous_time(*this));
    }
}
