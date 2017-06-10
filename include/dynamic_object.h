#ifndef GUARD_dynamic_object_h
#define GUARD_dynamic_object_h

#include <memory>
#include <string>
#include "h5cpp.h"
#include "geometry/geometry.h"
#include "vec.h"
#include "object.h"

namespace qbox {

    class Field2D;
    class material;

    class dynamic_object: public object {
    public:
        dynamic_object(std::string name, const geometry& geometryType, const material_variant& mat, vec position, double theta = 0);
        dynamic_object(const geometry& geometryType, const material_variant& mat, vec position, double theta = 0);

        void move(const vec& dr);
        void rotate(double dtheta);
        void set_position(const vec &p);
        void set_theta(double theta);

        h5cpp::h5group get_group() const override;
        void write() const override;
        void write_current() const;
    };

}

#endif
