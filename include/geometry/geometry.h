#ifndef GUARD_geometry_h
#define GUARD_geometry_h

#include <memory>
#include "vec.h"
#include "h5cpp.h"


///Geometry
//
namespace qbox {

    class geometry {
    public:
        geometry() = default;
        geometry (const geometry& other) { 
            bounding_box = other.bounding_box ? std::unique_ptr<vol>(new vol(*other.bounding_box)) : nullptr;
            interior_box = other.interior_box ? std::unique_ptr<vol>(new vol(*other.interior_box)) : nullptr;
        }

        virtual bool inside(const vec& v) const = 0;
        virtual void write(h5cpp::h5group& group) const = 0;
        virtual std::unique_ptr<geometry> clone() const = 0;

    protected:
        void set_bounding_box(const vec& a, const vec& b) {
            bounding_box = std::make_unique<vol>(a,b);
        }
        void set_interior_box(const vec& a, const vec& b) {
            interior_box = std::make_unique<vol>(a,b);
        }

    protected:
        std::unique_ptr<vol> bounding_box = nullptr;
        std::unique_ptr<vol> interior_box = nullptr;
    };

}

#endif
