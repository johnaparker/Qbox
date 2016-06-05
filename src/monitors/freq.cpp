#include <algorithm>
#include <math.h>
#include "monitors/freq.h"

using namespace std;

namespace qbox {

    freq_data::freq_data(): t(0), freq(nullptr), cosf(nullptr), sinf(nullptr), N(0) {}

    freq_data::freq_data(double f): N(N), t(0) {
        freq = new double[1];
        cosf = new double[1];
        sinf = new double[1];
        freq[0] = f;
        cosf[0] = 1;
        sinf[0] = 0;
    }

    freq_data::freq_data(double fmin, double fmax, int N): N(N), t(0) {
        freq = new double[N];
        cosf = new double[N];
        sinf = new double[N];
        for (int i = 0; i != N; i ++) {
            freq[i] = fmin + (fmax-fmin)/(N-1.0)*i;
            cosf[i] = 1;
            sinf[i] = 1;
        }
    }

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
        std::swap(first.freq, second.freq); 
        std::swap(first.sinf, second.sinf); 
        std::swap(first.cosf, second.cosf); 

        std::swap(first.N, second.N); 
        std::swap(first.t, second.t); 
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
