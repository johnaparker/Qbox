#ifndef GUARD_freq_h
#define GUARD_freq_h

#include <memory>
#include <cstring>
#include <vector>

namespace qbox {

    class Field2D;

    //Memory-control class for frequency data
    class freq_data {
    public:
        //Various ways to construct
        freq_data();
        freq_data(double f);    //single frequency
        freq_data(double fmin, double fmax, int N);    //N frequencies between fmin,fmax
        freq_data(double* f, int N);     //raw frequency array, size N
        
        //Rule of Five
        freq_data(const freq_data& rhs);
        freq_data& operator=(freq_data rhs);
        freq_data(freq_data&& rhs);
        freq_data& operator=(freq_data&& rhs);
        ~freq_data();
        friend void swap(freq_data& first, freq_data& second);

        //update cosf, sinf arrays with t = tnew
        void update(double tnew);

        //getter functions
        std::vector<double>  get_freq() {
            std::vector<double> ret(freq, freq+N);
            return ret;
        }
        double get_freq(int i) {return freq[i];}
        double get_cosf(int i) {return cosf[i];}
        double get_sinf(int i) {return sinf[i];}
        int size() {return N;}

    private:
        int N;   //size of arrays
        double t;   //current time
        double *freq;  //freq array
        double *cosf, *sinf;  //sin(ft), cos(ft) array
    };
}

#endif
