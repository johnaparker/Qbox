#ifndef GUARD_object_h
#define GUARD_object_h

#include <vector>

class object {
public:
    double eps;
    double conduc;
public:
    object();
    virtual bool inside(std::vector<double> p) {};
    void set_eps(double e);
    void set_conduc(double c);
};


class medium: public object {
public:
    medium();
    bool inside(std::vector<double> p);
};


class cylinder: public object {
public:
    double x,y,r;
public:
    cylinder(double x, double y, double r);
    bool inside(std::vector<double> p);
};

class rectangle: public object {
public:
    std::vector<double> pa, pb;
public:
    rectangle(std::vector<double> pa, std::vector<double> pb);
    bool inside(std::vector<double> p);
};


#endif
