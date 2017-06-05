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

    class dynamic_object {
    public:
        dynamic_object(std::string name, const geometry& geometryType, const material_variant& mat, vec position, double theta = 0);
        dynamic_object(const geometry& geometryType, const material_variant& mat, vec position, double theta = 0);

        bool inside(const vec& p) const;

        void move(const vec& dr);
        void rotate(double dtheta);

        void set_position(const vec &p);
        void set_theta(double theta);

        h5cpp::h5group get_group() const;
        void write() const;
        void write_material() const;

        std::unique_ptr<geometry> get_geometry() {return geometryType->clone();}
        material_variant get_material() const {return mat;}

        void set_owner(Field2D* F);

    private:
        std::string name;

        std::unique_ptr<geometry> geometryType;
        material_variant mat;
        vec position;
        double theta;

        std::unique_ptr<h5cpp::h5file> outFile = nullptr;

        static int num_created;
    };

}

#endif
