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
}

#endif
