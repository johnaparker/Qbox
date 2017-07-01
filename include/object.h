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

#include "dft.h"

namespace qbox {

    class Field2D;
    class material;

    template<class T> class box_monitor;

    using material_variant = std::variant<simple_material,debye,drude,lorentz>;

    class object {
    public:
        object(std::string name, const geometry& geometryType, const material_variant& mat, vec position, double theta = 0);
        object(const geometry& geometryType, const material_variant& mat, vec position, double theta = 0);

        bool inside(const vec& p) const;

        virtual h5cpp::h5group get_group() const;
        virtual void write() const;
        void write_material() const;

        std::optional<volume> get_bounding_box() const;
        std::unique_ptr<geometry> get_geometry() const {return geometryType->clone();}
        material_variant get_material() const {return mat;}
        std::unique_ptr<material> get_material_base() const {
            return std::visit([](auto&& arg){return arg.clone();}, mat);
        }

        box_monitor<DFT::all>& get_box_monitor(const Array& freq, int buffer = 1);

        void set_owner(Field2D* F);

    protected:
        std::string name;

        std::unique_ptr<geometry> geometryType;
        material_variant mat;
        vec position;
        double theta;

        Field2D* F;

        static int num_created;
    };

}

#endif
