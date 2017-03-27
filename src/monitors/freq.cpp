#include "monitors/freq.h"

using namespace std;

namespace qbox {

    freq_data::freq_data(const Array &freq): t(0), freq(freq) {
        sinf = Array::Zero(freq.size());
        cosf = Array::Ones(freq.size());
    }

    freq_data::freq_data(double f): freq_data(Array::Constant(1,f)) {}

    freq_data::freq_data(double fmin, double fmax, int N): freq_data(Array::LinSpaced(N, fmin, fmax)) {}

    void freq_data::update(double tnew) {
        if (t == tnew)
            return;

        t = tnew;
        cosf = Eigen::cos(2*M_PI*freq*t);
        sinf = Eigen::sin(2*M_PI*freq*t);
    }
}
