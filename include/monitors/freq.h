#ifndef GUARD_freq_h
#define GUARD_freq_h

#include "vec.h"

namespace qbox {

    //Memory-control class for frequency data
    class freq_data {
    public:
        freq_data() = default;
        freq_data(const Eigen::ArrayXd &freq);    ///< custom frequency
        freq_data(double fmin, double fmax, int N);    ///< N frequencies between fmin,fmax
        freq_data(double f);    ///< single frequency

        //update cosf, sinf arrays with t = tnew
        void update(double tnew);

        //getter functions
        Eigen::ArrayXd get_freq() const {return freq;}
        double get_freq(int i) const {return freq[i];}
        double get_cosf(int i) const {return cosf[i];}
        double get_sinf(int i) const {return sinf[i];}
        int size() const {return freq.size();}

    private:
        double t;   ///< current time
        Eigen::ArrayXd freq;   ///< frequency data
        Eigen::ArrayXd cosf;   ///< cosine values
        Eigen::ArrayXd sinf;   ///< sine values
    };

}


#endif
