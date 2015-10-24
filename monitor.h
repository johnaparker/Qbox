#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <vector>
#include <string>
#include "matrix.h"
#include "field2.h"

//technically, these are DFT monitors. Normal monitor can exist too.
class monitor {
public:
    std::string name;             //to call output from main
    int N;
    double *freq;
    double *prevE;
public:
    monitor();
    virtual void update();
    virtual void output();
};

//add friendship to Field2D
class surface_monitor: public monitor {
public:
    std::vector<int> p1, p2;
    matrix<double> rE, iE, rH, iH;
public:
    surface_monitor(std::vector p1, std::vector p2, double *freq, int N);
    surface_monitor(std::vector p1, std::vector p2, double fmin, double fmax, int N);
    surface_monitor(std::vector p1, std::vector p2, double f);
    void update(Field2D & F);
    void output();
};





#endif
