#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <vector>
#include <string>
#include "matrix.h"
#include "field2.h"

class Field2D;

//technically, these are DFT monitors. Normal monitor can exist too.
class monitor {
public:
    std::string name;             //to call output from main
    int N;
    double *freq;
    double *prevE;
    Field2D *F;
public:
    monitor() = default;
    monitor(std::string name, double *freq, int N): name(name), freq(freq), N(N) {};
    virtual void set_freq(double *new_freq);
    virtual void set_F(Field2D *newF);
    virtual void update() {};
};

//add friendship to Field2D
//note that this computes flux in the +x/y direction, but could be smarter to do p/m
//or get E/B values from base class
class surface_monitor: public monitor {
public:
    std::vector<double> p1d, p2d;
    std::vector<int> p1, p2;
    matrix<double> rE, iE, rH, iH;
    int dir;
    int length;
    //move these into base class. Find out a way to not compute 4x
    double *sinf, *cosf;
public:
    surface_monitor(std::string name, std::vector<double> p1, std::vector<double> p2, double *freq, int N);
    surface_monitor(std::string name, std::vector<double> p1, std::vector<double> p2, double fmin, double fmax, int N);
    surface_monitor(std::string name, std::vector<double> p1, std::vector<double> p2, double f);
    surface_monitor() = default;
    void update();
    void set_F(Field2D *newF);
    void compute_flux(double *S);
    void write(std::string filename, bool extendable = false);
};

class box_monitor: public monitor {
public:
    surface_monitor monitors[4];
public:
    box_monitor(std::string name, std::vector<double> p1, std::vector<double> p2, double *freq, int N);
    box_monitor(std::string name, std::vector<double> p1, std::vector<double> p2, double fmin, double fmax, int N);
    box_monitor(std::string name, std::vector<double> p1, std::vector<double> p2, double f);
    box_monitor() = default;
    void set_freq(double *new_freq);
    void set_F(Field2D *newF);
    void update();
    void compute_flux(double *S);
    void write(std::string filename, bool extendable = false);
    void write_sides(std::string filename, bool extendable = false);
};



#endif
