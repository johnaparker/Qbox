#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "h5out.h"

using namespace std;

int get_direction(vector<int> p1, vector<int> p2) {
    if (p1[0] == p2[0])
        return 1;
    else if (p1[1] == p2[1])
        return 0;
    else
        return 2;
}

void monitor::set_freq(shared_ptr<freq_data> new_freq) {
    //freq = unique_ptr<double[]> (new double(*new_freq));
    freq = new_freq;
}

void monitor::set_F(Field2D *newF) {
    F = newF;
}


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

surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, shared_ptr<freq_data> freq, int N): monitor(name,freq,N), p1(p1), p2(p2), dir(0), length(0) {
    F = nullptr;
    prevE = nullptr;
}

surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double fmin, double fmax, int N): surface_monitor(name, p1, p2, nullptr, N) {
    freq = shared_ptr<freq_data> (new freq_data(fmin, fmax, N));
}

surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double f): 
        surface_monitor(name, p1, p2, nullptr, 1) {
    freq = shared_ptr<freq_data> (new freq_data(f));
}

void surface_monitor::set_F(Field2D *newF) {
    monitor::set_F(newF);
    p1g = (F->grid).convertToGrid(p1);
    p2g = (F->grid).convertToGrid(p2);
    dir = get_direction(p1g, p2g);
    length = p2g[dir] - p1g[dir];
    prevE = unique_ptr<double[]>(new double[length+1]);
    rE = matrix<double>(length, N);
    iE = matrix<double>(length, N);
    rH = matrix<double>(length, N);
    iH = matrix<double>(length, N);
    for (int i = 0; i != length; i++) {
        prevE[i] = 0;
        for (int j = 0; j!= N; j++) {
            rE[i][j] = 0;
            iE[i][j] = 0;
            rH[i][j] = 0;
            iH[i][j] = 0;
        }
    }
    prevE[length] = 0;
}

void surface_monitor::update() {
    matrix<double> *Hfield;
    if (dir == 0)
        Hfield = &F->Hx;
    if (dir == 1)
        Hfield = &F->Hy;

    int a = p1g[0];
    int b = p1g[1];
    double E, H;
    (*freq).update(F->t);

    //this if check could be done outside the for loop somehow
    for (int i = 0; i != length; i++) {
        if (dir == 0) {
            a = p1g[0] + i;
            H = ((*Hfield)[a][b] + (*Hfield)[a][b-1]
                    + (*Hfield)[a+1][b] + (*Hfield)[a+1][b-1])/4;
            E = (F->Ez[a][b] + F->Ez[a+1][b]
                    + prevE[i] + prevE[i+1])/4;
        }
        else if (dir == 1) {
            b = p1g[1] + i;
            H = ((*Hfield)[a][b] + (*Hfield)[a-1][b]
                    + (*Hfield)[a][b+1] + (*Hfield)[a-1][b+1])/4;
            E = (F->Ez[a][b] + F->Ez[a][b+1]
                    + prevE[i] + prevE[i+1])/4;
        }
        prevE[i] = F->Ez[a][b];

        for (int j = 0; j != N; j++) {
            rE[i][j] += E*(*freq).get_cosf(j);
            iE[i][j] += E*(*freq).get_sinf(j);
            rH[i][j] += H*(*freq).get_cosf(j);
            iH[i][j] += H*(*freq).get_sinf(j);
        }
    }
    if (dir == 0)
        a += 1;
    else
        b += 1;
    prevE[length] = F->Ez[a][b];
}

void surface_monitor::compute_flux(double *S) {
    for (int i = 0; i != N; i++) 
        S[i] = 0;

    for (int j = 0; j != N; j++) {
        for (int i = 0; i != length; i++) {
            S[j] += rE[i][j]*rH[i][j] + iE[i][j]*iH[i][j];
        }
        S[j] *= F->dx;
    }
}

void surface_monitor::write(string filename, bool extendable) {
    double *S = new double[N];
    compute_flux(S);
    F->write_monitor(filename, name, S, N, extendable); 
    delete[] S;
}



box_monitor::box_monitor(string name, vector<int> p1, vector<int> p2, shared_ptr<freq_data> freq_in, int N):
                monitor(name, freq_in, N) {
    
    monitors[0] = move(surface_monitor(name + "_1", p1, {p2[0], p1[1]}, freq, N));
    monitors[1] = move(surface_monitor(name + "_2", {p2[0], p1[1]}, p2, freq, N));
    monitors[2] = move(surface_monitor(name + "_3", {p1[1], p2[0]}, p2, freq, N));
    monitors[3] = move(surface_monitor(name + "_4", p1, {p1[1], p2[0]}, freq, N));
}

box_monitor::box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N):
            box_monitor(name, p1, p2, nullptr, N) {
    freq = shared_ptr<freq_data> (new freq_data(fmin, fmax, N));
    set_freq(freq);
}

box_monitor::box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f):
            box_monitor(name, p1, p2, nullptr, 1) {
    freq = shared_ptr<freq_data>(new freq_data(f));
    set_freq(freq);
}

void box_monitor::set_freq(shared_ptr<freq_data> new_freq) {
    monitor::set_freq(new_freq);
    for (int i = 0; i != 4; i++) 
        monitors[i].set_freq(new_freq);
}

void box_monitor::set_F(Field2D *newF) {
    monitor::set_F(newF);
    for (int i = 0; i != 4; i++) 
        monitors[i].set_F(newF);
}

void box_monitor::update() {
    for (int i = 0; i != 4; i++) 
        monitors[i].update();
}

void box_monitor::compute_flux(double *S) {
    double sign_values[] = {1, 1, -1, -1};
    double *Sm = new double[N];
    for (int i = 0; i != N; i++) {
        S[i] = 0;
        Sm[i] = 0;
    }

    for (int i = 0; i != 4; i++) {
        monitors[i].compute_flux(Sm);
        for (int j = 0; j != N; j++)
            S[j] += sign_values[i]*Sm[j];
    }
    delete[] Sm;
}

void box_monitor::write(string filename, bool extendable) {
    double *S = new double[N];
    compute_flux(S);
    F->write_monitor(filename, name, S, N, extendable); 
    delete[] S;
}

void box_monitor::write_sides(string filename, bool extendable) {
    for (int i = 0; i != 4; i++) 
        monitors[i].write(filename, extendable);
}

