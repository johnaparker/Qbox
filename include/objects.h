#ifndef GUARD_object_h
#define GUARD_object_h

#include <memory>
#include <string>
#include "h5cpp.h"
#include "geometry/geometry.h"
#include "materials/material.h"
#include "vec.h"

namespace qbox {

    class object {
    public:
        object(std::shared_ptr<geometry> geometryType, std::shared_ptr<material> materialType, vec center, vec orientation = vec(0,0,0));

        bool inside(const vec& v) const;

        void move(const vec& dr);
        void rotate(quat rot);

        void write();

    private:
        vec center;
        vec orientation;

        std::shared_ptr<geometry> geometryType;
        std::shared_ptr<material> materialType;
        std::unique_ptr<h5cpp::h5dset> pos_dset;
        std::unique_ptr<h5cpp::h5dset> rot_dset;
        std::unique_ptr<h5cpp::h5file> file;
    };

}

#endif
