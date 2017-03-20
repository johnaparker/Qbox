#ifndef GUARD_geometry_h
#define GUARD_geometry_h

#include <iostream>
#include <vector>
#include <math.h>
#include <memory>
#include <vec.h>
#include "h5cpp.h"


///Geometry
//
namespace qbox {
    class geometry {
    public:
        virtual bool inside(const vec& v) const = 0;
        virtual void write(std::unique_ptr<h5cpp::h5file>& file) const = 0;
    protected:
        unique_ptr<vol> bounding_box = nullptr;
        unique_ptr<vol> interior_box = nullptr;
        void set_bounding_box(const vec& a, const vec& b) {
            bounding_box = make_unique<vol>(a,b);
        }
        void set_interior_box(const vec& a, const vec& b) {
            interior_box = make_unique<vol>(a,b);
        }
    };

}

#endif
