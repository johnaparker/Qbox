#ifndef GUARD_material_h
#define GUARD_material_h

#include <memory>

namespace qbox {

    class material {
    public:
        virtual double get_eps() = 0;
        virtual double get_mu() = 0;
        virtual double get_conduc() = 0;

        virtual std::unique_ptr<material> clone() const = 0;
    };

}

#endif

