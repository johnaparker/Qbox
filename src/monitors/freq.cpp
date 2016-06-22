#include <algorithm>
#include <math.h>
#include "monitors/freq.h"

using namespace std;

namespace qbox {

    freq_data::freq_data(): N(0), t(0), freq(nullptr), cosf(nullptr), sinf(nullptr) {}


    freq_data::freq_data(double* freq_in, int N): N(N), t(0) {
        freq = new double[N];
        cosf = new double[N];
        sinf = new double[N];
        for (int i = 0; i != N; i ++) {
            freq[i] = freq_in[i];
            cosf[i] = 1;
            sinf[i] = 0;
        }
    }

    freq_data::freq_data(double fmin, double fmax, int N): N(N), t(0) {
        freq = new double[N];
        cosf = new double[N];
        sinf = new double[N];
        double d_freq = (fmax-fmin)/(N-1) ? N != 1 : 0;
        for (int i = 0; i != N; i ++) {
            freq[i] = fmin + d_freq*i;
            cosf[i] = 1;
            sinf[i] = 0;
        }
    }

    freq_data::freq_data(double f): freq_data(f,f,1) {}

    freq_data::~freq_data() {
        delete[] freq;
        delete[] cosf;
        delete[] sinf;
    }

    freq_data::freq_data(const freq_data& rhs): N(rhs.N), t(rhs.t) {
        if (N == 0) {
            freq = nullptr;
            cosf = nullptr;
            sinf = nullptr;
        }
        else {
            freq = new double[N];
            cosf = new double[N];
            sinf = new double[N];
            for (int i = 0; i != N; i ++) {
                freq[i] = rhs.freq[i];
                cosf[i] = rhs.cosf[i];
                sinf[i] = rhs.sinf[i];
            }
        }
    }

    void swap(freq_data& first, freq_data& second) {
        using std::swap;

        swap(first.freq, second.freq); 
        swap(first.sinf, second.sinf); 
        swap(first.cosf, second.cosf); 

        swap(first.N, second.N); 
        swap(first.t, second.t); 
    }

    freq_data& freq_data::operator= (freq_data rhs) {
        swap(*this, rhs);
        return *this;
    }

    freq_data::freq_data(freq_data&& rhs): N(rhs.N), t(rhs.t) {
        freq = rhs.freq;
        sinf = rhs.sinf;
        cosf = rhs.cosf;
        N = rhs.N;

        rhs.N = 0;
        rhs.freq = nullptr;
        rhs.sinf = nullptr;
        rhs.cosf = nullptr;
    }

    freq_data& freq_data::operator= (freq_data&& rhs) {
        if (this != &rhs) {
            delete[] freq;
            delete[] sinf;
            delete[] cosf;

            freq = rhs.freq;
            sinf = rhs.sinf;
            cosf = rhs.cosf;
            N = rhs.N;
            t = rhs.t;

            rhs.N = 0;
            rhs.t = 0;
            rhs.freq = nullptr;
            rhs.sinf = nullptr;
            rhs.cosf = nullptr;
        }
        return *this;
    }

    void freq_data::update(double tnew) {
        if (t == tnew)
            return;

        t = tnew;
        for (int j = 0; j != N; j++) {
            cosf[j] = cos(2*M_PI*freq[j]*t);
            sinf[j] = sin(2*M_PI*freq[j]*t);
        }
    }
}
