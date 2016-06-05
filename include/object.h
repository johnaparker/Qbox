#ifndef GUARD_object_h
#define GUARD_object_h

#include <vector>

namespace apine {
    //object base class
    class object {
    public:
        object();

        //bool function to determine whether the vector position p is inside the object; defines the geometry
        virtual bool inside(std::vector<int> p) {};

        //set eps/conduc
        void set_eps(double e);
        void set_conduc(double c);

    public:
        double eps;      //relative permittivity of object
        double conduc;   //conductivity of object
    };


    //medium object: special object for background medium
    class medium: public object {
    public:
        medium();
        bool inside(std::vector<int> p);
    };

    //cylinder object
    class cylinder: public object {
    public:
        cylinder(double x, double y, double r);
        bool inside(std::vector<int> p);
    public:
        //*** should be vec
        double x,y,r;   //x,y position and radius r
    };

    //rectangle object
    class rectangle: public object {
    public:
        rectangle(std::vector<double> pa, std::vector<double> pb);
        bool inside(std::vector<int> p);
    public:
        //*** should be volume
        std::vector<double> pa, pb;   //vector corners
    };
}

#endif
