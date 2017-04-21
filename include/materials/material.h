#ifndef GUARD_material_h
#define GUARD_material_h

#include <memory>
#include <string>
#include "h5cpp.h"

namespace qbox {

    class material {
    public:
        material(std::string name, std::string group_name): name(name), group_name(group_name) {};
        material(std::string group_name): material("material_" + std::to_string(num_created), group_name) {num_created++;}

        virtual double Ca(double dt) const = 0;
        virtual double Cb(double dt) const = 0;
        virtual double Da(double dt) const = 0;
        virtual double Db(double dt) const = 0;

        h5cpp::h5group get_group(const h5cpp::h5file &outFile) const;
        virtual void write(const h5cpp::h5file &outFile) const = 0;

        std::string get_name() const {return name;}

        virtual std::unique_ptr<material> clone() const = 0;

    protected:
        std::string name;
        std::string group_name;

    private:
        static int num_created;
    };
}

#endif

