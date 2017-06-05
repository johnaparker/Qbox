#ifndef GUARD_object_h
#define GUARD_object_h

#include <memory>
#include <string>
#include "h5cpp.h"
#include "geometry/geometry.h"
#include "vec.h"
#include <variant>

#include "materials/simple_material.h"
#include "materials/debye.h"
#include "materials/drude.h"
#include "materials/lorentz.h"

namespace qbox {

    class Field2D;
    class material;

    using material_variant = std::variant<simple_material,debye,drude,lorentz>;

    class object {
    public:
        object(std::string name, const geometry& geometryType, const material_variant& mat, vec position, vec orientation = vec(0,1));
        object(const geometry& geometryType, const material_variant& mat, vec position, vec orientation = vec(0,1));

        bool inside(const vec& p) const;

        void move(const vec& dr);
        void rotate(quat rot);

        h5cpp::h5group get_group() const;
        void write() const;
        void write_material() const;

        std::unique_ptr<geometry> get_geometry() const {return geometryType->clone();}
        material_variant get_material() const {return mat;}

        void set_owner(Field2D* F);

    private:
        std::string name;

        std::unique_ptr<geometry> geometryType;
        material_variant mat;
        vec position;
        vec orientation;

        std::unique_ptr<h5cpp::h5file> outFile = nullptr;

        static int num_created;
    };

}

#endif
