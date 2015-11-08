#include "source.h"
#include "field2.h"
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

//generalization: allow Ex,Hx,Hy, etc. excitations
//          look into this using an enum
//add TFSF support
//reduce code copying
//  maybe have line/point classes inherent from source, and then further class inherent from these
//  and use functions for time profiles
//best way to orient the line
//
//Allow amplitude specfications; allow gradual turn on for sine
//For Gaussian, get rid of T0, in favor of starting it at specified time

source::source() {
    F = nullptr;
    t = nullptr;
}
void source::set_F(Field2D *field) {
    F = field;
    t = &(F->t);
}

continuous_point_source::continuous_point_source(vector<int> p, double freq): p(p), freq(freq) {};
void continuous_point_source::pulse() {
    static vector<int> pi = (F->grid).convertToGrid(p);
    F->Ez[pi[0]][pi[1]] += sin(2*M_PI*freq*(*t));
}

gaussian_point_source::gaussian_point_source(vector<int> p, double T0, double sig):
    p(p), T0(T0), sig(sig) {};
void gaussian_point_source::pulse() {
    static vector<int> pi = (F->grid).convertToGrid(p);
    F->Ez[pi[0]][pi[1]] += (F->grid).res*exp(-0.5*(pow((*t-T0)/sig,2)));
}

custom_point_source::custom_point_source(vector<int> p, double (*time_func)(double)):
    p(p), time_func(time_func) {};
void custom_point_source::pulse() {
    static vector<int> pi = (F->grid).convertToGrid(p);
    F->Ez[pi[0]][pi[1]] += time_func(*t);
}
    
continuous_line_source::continuous_line_source(vector<int> p1, vector<int> p2, double freq): p1(p1), p2(p2), freq(freq) {};
void continuous_line_source::pulse() {
    static vector<int> p1i = (F->grid).convertToGrid(p1);
    static vector<int> p2i = (F->grid).convertToGrid(p2);

    int start, end;
    bool vertical = false;
    if (p1i[0] == p2i[0]) {
        start = p1i[1];
        end = p2i[1];
        vertical = true;
    }
    else if (p1i[1] == p2i[1]) {
        start = p1i[0];
        end = p2i[0];
    }
    double pulse_amp = sin(2*M_PI*freq*(*t));
    for (int i = start; i != end; i++) {
        if (vertical)
            F->Ez[p1i[0]][i] += pulse_amp;
        else
            F->Ez[i][p2i[1]] += pulse_amp;
    }
}

gaussian_line_source::gaussian_line_source(vector<int> p1, vector<int> p2, double T0, double sig):
    p1(p1), p2(p2),T0(T0), sig(sig) {};
void gaussian_line_source::pulse() {
}

custom_line_source::custom_line_source(vector<int> p1, vector<int> p2, double (*time_func)(double)):
    p1(p1), p2(p2), time_func(time_func) {};
void custom_line_source::pulse() {
}
