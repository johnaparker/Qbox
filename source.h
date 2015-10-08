#ifndef GUARD_source_h
#define GUARD_source_h

#include <vector>
#include <fstream>
#include "matrix.h"
#include "object.h"
#include "field.h"

//Think about whether this is the best way to do sources
//Also think about how to add TFSF into these sources
//Lastly, the best way to couple these sources with the field class.



class source {
public:
    source();
    virtual void pulse(); 
};

class continuous_point_source: public source {
public:
    int x, y;
    double f;
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
    double (*f)(double);
public:
    custom_point_source(int x, int y, double (*f)(double));
    void pulse();
};



#endif
