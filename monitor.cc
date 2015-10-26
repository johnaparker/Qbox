#include <vector>
#include <stirng>
#include "matrix.h"
#include "field2.h"

using namespace std;

int get_direction(vector<int> p1, vector<int> p2) {
    if (p1[0] == p2[0])
        return 1;
    else if (p1[1] == p2[1])
        return 0;
    else
        return 2;
}

surface_monitor(string name, vector<int> p1, vector<int> p2, double *freq, int N): name(name), freq(freq), p1(p1), p2(p2), N(N) {
    F = nullptr;
    dir = get_direction(p1, p2);
    length = p2[dir] - p1[dir] + 1;
    prevE = new double[length];
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
}

surface_monitor(string name, vector<int> p1, vector<int> p2, double fmin, double fmax, int N): 
            surface_monitor(name, p1, p2, nullptr, N) {
    freq = new double[N];
    for (int i = 0; i != N; i ++) {
        freq[i] = p1[dir] + (p2[dir]-p1[dir])/(N-1)*i;
    }
}

surface_monitor(string name, vector<int> p1, vector<int> p2, double f): 
            surface_monitor(name, p1, p2, nullptr, 1) {
    freq = new double[1];
    freq[0] = f;
}


surface_monitor::update() {
    matrix<double> *Hfield;
    if (dir == 0)
        Hfield = &F->Hx;
    if (dir == 1)
        Hfield = &F->Hy;

    int a = p1[0];
    int b = p2[0];
    //this if check could be done outside the for loop somehow
    for (int i = 0; i != length; i++) {
        if (dir == 0) {
            a = p1[0] + i;
            double E = (prevE[i] + F->Ez[a][b])/2;
            double H = ((*Hfield)[a][b] + (*Hfield)[a-1][b-1])/2;
        }
        else if (dir == 1) {
            b = p1[1] + i;
            double E = (prevE[i] + F->Ez[a][b])/2;
            double H = ((*Hfield)[a][b] + (*Hfield)[a][b-1])/2;
        }
        prevE[i] = F->Ez[a][b]/2;

        for (int j = 0; j != N; j++) {
            rE[i][j] += E*cos(2*M_PI*f[j]*F->t);
            iE[i][j] += E*sin(2*M_PI*f[j]*F->t);
            rH[i][j] += H*cos(2*M_PI*f[j]*F->t);
            iH[i][j] += H*sin(2*M_PI*f[j]*F->t);
        }
    }
}

void write(string filename) {
    double *S = new double[N];
    for (int i = 0; i != N; i++) 
        S[i] = 0;

    for (int i = 0; i != length; i++) {
        for (int j = 0; j != N; j++) {
            S[j] += rE[i][j]*rH[i][j] - iE[i][j]*iH[i][j];       //multiply by the length somewhere
        }
    }
    F->write(filename, name);            //figure out how to pass S to  F for writing

    delete[] S;
}




            
