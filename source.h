#ifndef GUARD_source_h
#define GUARD_source_h

#include <vector>
#include "field2.h"

//Think about whether this is the best way to do sources
//Also think about how to add TFSF into these sources
//Lastly, the best way to couple these sources with the field class.

class Field2D;

class source {
public:
    Field2D *F;
    double *t;
public:
    source();
    void set_F(Field2D *F);
    virtual void pulse() {}; 
};

class continuous_point_source: public source {
public:
    int x, y;
    double freq;
public:
    continuous_point_source(int x, int y, double f);
    void pulse();
};

class gaussian_point_source: public source {
public:
    int x, y;
    double T0, sig;
public:
    gaussian_point_source(int x, int y, double T0, double sig);
    void pulse();
};

class custom_point_source: public source {
public:
    int x, y;
    double (*time_func)(double);
public:
    custom_point_source(int x, int y, double (*time_func)(double));
    void pulse();
};


class continuous_line_source: public source {
public:
    std::vector<int> p1, p2;
    double freq;
public:
    continuous_line_source(std::vector<int> p1, std::vector<int> p2, double f);
    void pulse();
};

class gaussian_line_source: public source {
public:
    std::vector<int> p1, p2;
    double T0, sig;
public:
    gaussian_line_source(std::vector<int> p1, std::vector<int> p2, double T0, double sig);
    void pulse();
};

class custom_line_source: public source {
public:
    std::vector<int> p1, p2;
    double (*time_func)(double);
public:
    custom_line_source(std::vector<int> p1, std::vector<int> p2, double (*time_func)(double));
    void pulse();
};

#endif
