#ifndef GUARD_object_h
#define GUARD_object_h

#include <memory>
#include "h5cpp.h"
#include "geometry/geometry.h"
#include "materials/material.h"
#include "vec.h"

namespace qbox {

    class object {
    public:
        object(geometry& geometryType, material& materialType, vec center, vec orientation = vec(0,1));

        bool inside(const vec& p) const;

        void move(const vec& dr);
        void rotate(quat rot);

        void write();

        std::unique_ptr<geometry> get_geometry() {return geometryType->clone();}
        std::unique_ptr<material> get_material() {return materialType->clone();}

    private:
        vec center;
        vec orientation;

        std::unique_ptr<geometry> geometryType;
        std::unique_ptr<material> materialType;
    };

}

#endif
