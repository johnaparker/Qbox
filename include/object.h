#ifndef GUARD_object_h
#define GUARD_object_h

#include <memory>
#include <string>
#include "h5cpp.h"
#include "geometry/geometry.h"
#include "vec.h"

namespace qbox {

    class Field2D;

    class object {
    public:
        object(std::string name, const geometry& geometryType, vec position, vec orientation = vec(0,1));
        object(const geometry& geometryType, vec position, vec orientation = vec(0,1));

        bool inside(const vec& p) const;

        void move(const vec& dr);
        void rotate(quat rot);

        void write();

        std::unique_ptr<geometry> get_geometry() {return geometryType->clone();}

        void set_owner(Field2D* F);

    private:
        std::string name;

        std::unique_ptr<geometry> geometryType;
        vec position;
        vec orientation;

        std::unique_ptr<h5cpp::h5file> outFile = nullptr;

        static int num_created;
    };

}

#endif
