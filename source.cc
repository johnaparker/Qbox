#include "source.h"
#include "field2.h"
#include <math.h>

using namespace std;

//generalization: allow Ex,Hx,Hy, etc. excitations
//add TFSF support
//reduce code copying
//best way to orient the line

source::source() {
    F = nullptr;
    t = nullptr;
}
void source::set_F(Field2D *field) {
    F = field;
    t = &(F->t);
}

continuous_point_source::continuous_point_source(int x, int y, double freq): x(x), y(y), freq(freq) {};
void continuous_point_source::pulse() {
    F->Ez[x][y] += sin(2*M_PI*freq*(*t));
}

gaussian_point_source::gaussian_point_source(int x, int y, double T0, double sig):
    x(x), y(y),T0(T0), sig(sig) {};
void gaussian_point_source::pulse() {
    F->Ez[x][y] += exp(-0.5*(pow((*t-T0)/sig,2)));
}

custom_point_source::custom_point_source(int x, int y, double (*time_func)(double)):
    x(x), y(y), time_func(time_func) {};
void custom_point_source::pulse() {
    F->Ez[x][y] += time_func(*t);
}
    
continuous_line_source::continuous_line_source(vector<int> p1, vector<int> p2, double freq): p1(p1), p2(p2), freq(freq) {};
void continuous_line_source::pulse() {
    int start, end;
    bool vertical = false;
    if (p1[0] == p2[0]) {
        start = p1[1];
        end = p2[1];
        vertical = true;
    }
    else if (p1[1] == p2[1]) {
        start = p1[0];
        end = p2[0];
    }
    for (int i = start; i != end; i++) {
        if (vertical)
            F->Ez[p1[0]][i] += sin(2*M_PI*freq*(*t));
        else
            F->Ez[i][p2[1]] += sin(2*M_PI*freq*(*t));
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
