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

void monitor::set_freq(double *new_freq) {
    freq = new_freq;
}

void monitor::set_F(Field2D *newF) {
    F = newF;
}

surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double *freq, int N): monitor(name,freq,N), p1(p1), p2(p2) {
    F = nullptr;
    cosf = new double[N];
    sinf = new double[N];
    dir = get_direction(p1, p2);
    length = p2[dir] - p1[dir];
    prevE = new double[length+1];
    rE = matrix<double>(new double[N*length], length, N);
    iE = matrix<double>(new double[N*length], length, N);
    rH = matrix<double>(new double[N*length], length, N);
    iH = matrix<double>(new double[N*length], length, N);
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

surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double fmin, double fmax, int N): 
            surface_monitor(name, p1, p2, nullptr, N) {
    freq = new double[N];
    for (int i = 0; i != N; i ++) {
        freq[i] = fmin + (fmax-fmin)/(N-1.0)*i;
    }
}

surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double f): 
            surface_monitor(name, p1, p2, nullptr, 1) {
    freq = new double[1];
    freq[0] = f;
}


void surface_monitor::update() {
    matrix<double> *Hfield;
    if (dir == 0)
        Hfield = &F->Hx;
    if (dir == 1)
        Hfield = &F->Hy;

    int a = p1[0];
    int b = p1[1];
    double E, H;
    for (int j = 0; j != N; j++) {
        cosf[j] = cos(2*M_PI*freq[j]*F->t);
        sinf[j] = sin(2*M_PI*freq[j]*F->t);
    }
    //this if check could be done outside the for loop somehow
    for (int i = 0; i != length; i++) {
        if (dir == 0) {
            a = p1[0] + i;
            H = ((*Hfield)[a][b] + (*Hfield)[a][b-1]
                    + (*Hfield)[a+1][b] + (*Hfield)[a+1][b-1])/4;
            E = (F->Ez[a][b] + F->Ez[a+1][b]
                    + prevE[i] + prevE[i+1])/4;
        }
        else if (dir == 1) {
            b = p1[1] + i;
            H = ((*Hfield)[a][b] + (*Hfield)[a-1][b]
                    + (*Hfield)[a][b+1] + (*Hfield)[a-1][b+1])/4;
            E = (F->Ez[a][b] + F->Ez[a][b+1]
                    + prevE[i] + prevE[i+1])/4;
        }
        prevE[i] = F->Ez[a][b];

        for (int j = 0; j != N; j++) {
            //HERE: STORE sin(f) and cos(f) seperately to reduce calls
            rE[i][j] += E*cosf[j];
            iE[i][j] += E*sinf[j];
            rH[i][j] += H*cosf[j];
            iH[i][j] += H*sinf[j];
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



box_monitor::box_monitor(string name, vector<int> p1, vector<int> p2, double *freq, int N):
                monitor(name, freq, N) {
    monitors[0] = surface_monitor(name + "_1", p1, {p2[0], p1[1]}, freq, N);
    monitors[1] = surface_monitor(name + "_2", {p2[0], p1[1]}, p2, freq, N);
    monitors[2] = surface_monitor(name + "_3", {p1[1], p2[0]}, p2, freq, N);
    monitors[3] = surface_monitor(name + "_4", p1, {p1[1], p2[0]}, freq, N);
}

box_monitor::box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N):
            box_monitor(name, p1, p2, nullptr, N) {
    freq = new double[N];
    for (int i = 0; i != N; i ++) {
        freq[i] = fmin + (fmax-fmin)/(N-1.0)*i;
    }
    set_freq(freq);
}

box_monitor::box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f):
            box_monitor(name, p1, p2, nullptr, 1) {
    freq = new double[1];
    freq[0] = f;
    set_freq(freq);
}

void box_monitor::set_freq(double *new_freq) {
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


















