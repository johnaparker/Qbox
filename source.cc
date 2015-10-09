#include "source.h"
#include "field2.h"
#include <math.h>

using namespace std;

//generalization: allow Ex,Hx,Hy, etc. excitations

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
    F->Dz[x][y] += sin(2*M_PI*freq*(*t));
}

gaussian_point_source::gaussian_point_source(int x, int y, double T0, double sig):
    x(x), y(y),T0(T0), sig(sig) {};
void gaussian_point_source::pulse() {
    F->Dz[x][y] += exp(-0.5*(pow((*t-T0)/sig,2)));
}

custom_point_source::custom_point_source(int x, int y, double (*time_func)(double)):
    x(x), y(y), time_func(time_func) {};
void custom_point_source::pulse() {
    F->Dz[x][y] += time_func(*t);
}
    
