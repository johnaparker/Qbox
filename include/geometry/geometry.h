#ifndef GUARD_geometry_h
#define GUARD_geometry_h

#include <memory>
#include "h5cpp.h"
#include "../vec.h"
#include <optional>

///Geometry
//
namespace qbox {

    class geometry {
    public:
        geometry() = default;

        virtual bool inside(const vec& v) const = 0;
        virtual void write(const h5cpp::h5group& group) const = 0;
        virtual std::unique_ptr<geometry> clone() const = 0;

        virtual std::string group_name() const = 0;

        virtual std::optional<volume> get_bounding_box(double theta) const;

        std::optional<volume> get_bounding_box() const {return bounding_box;}

    protected:
        void set_bounding_box(const vec& a, const vec& b) {
            bounding_box = volume(a,b);
        }

    private:
        std::optional<volume> bounding_box;
    };

}

#endif
