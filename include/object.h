#ifndef GUARD_object_h
#define GUARD_object_h

#include <memory>
#include <string>
#include "h5cpp.h"
#include "geometry/geometry.h"
#include "materials/material.h"
#include "vec.h"

namespace qbox {

    class Field2D;

    class object {
    public:
        object(std::string name, const geometry& geometryType, const material& materialType, vec position, vec orientation = vec(0,1));
        object(const geometry& geometryType, const material& materialType, vec position, vec orientation = vec(0,1));

        bool inside(const vec& p) const;

        void move(const vec& dr);
        void rotate(quat rot);

        void write();

        std::unique_ptr<geometry> get_geometry() {return geometryType->clone();}
        std::unique_ptr<material> get_material() {return materialType->clone();}

        void set_owner(Field2D* F);

    private:
        std::unique_ptr<geometry> geometryType;
        std::unique_ptr<material> materialType;

        vec position;
        vec orientation;

        std::string name;
        std::unique_ptr<h5cpp::h5file> outFile = nullptr;

        static int num_created;
    };

}

#endif
