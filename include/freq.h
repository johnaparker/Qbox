#ifndef GUARD_freq_h
#define GUARD_freq_h

namespace apine {

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
        double get_freq(int i) {return freq[i];}
        double get_cosf(int i) {return cosf[i];}
        double get_sinf(int i) {return sinf[i];}

    private:
        double *freq;  //freq array
        double *cosf, *sinf;  //sin(ft), cos(ft) array
        int N;   //size of arrays
        double t;   //current time
    };
}

#endif
