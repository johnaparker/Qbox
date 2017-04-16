#ifndef GUARD_material_h
#define GUARD_material_h

#include <memory>
#include "h5cpp.h"

namespace qbox {

    class material {
    public:
        virtual double Ca(double dt) const = 0;
        virtual double Cb(double dt) const = 0;
        virtual double Da(double dt) const = 0;
        virtual double Db(double dt) const = 0;

        virtual void write(const h5cpp::h5group &group) = 0;

        virtual std::unique_ptr<material> clone() const = 0;
    };

}

#endif

