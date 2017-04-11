#ifndef GUARD_geometry_h
#define GUARD_geometry_h

#include <memory>
#include "h5cpp.h"
#include "../vec.h"


///Geometry
//
namespace qbox {

    class geometry {
    public:
        geometry() = default;
        geometry (const geometry& other) { 
            bounding_box = other.bounding_box ? std::unique_ptr<volume>(new volume(*other.bounding_box)) : nullptr;
            interior_box = other.interior_box ? std::unique_ptr<volume>(new volume(*other.interior_box)) : nullptr;
        }

        virtual bool inside(const vec& v) const = 0;
        virtual void write(const h5cpp::h5group& group) const = 0;
        virtual std::unique_ptr<geometry> clone() const = 0;

        virtual std::string group_name() const = 0;

    protected:
        void set_bounding_box(const vec& a, const vec& b) {
            bounding_box = std::make_unique<volume>(a,b);
        }
        void set_interior_box(const vec& a, const vec& b) {
            interior_box = std::make_unique<volume>(a,b);
        }

    protected:
        std::unique_ptr<volume> bounding_box = nullptr;
        std::unique_ptr<volume> interior_box = nullptr;
    };

}

#endif
