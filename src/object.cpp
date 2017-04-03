#include "object.h"
#include <eigen3/Eigen/Geometry>
#include "math.h"
#include "field2.h"

using namespace std;

namespace qbox {

    int object::num_created = 0;

    object::object(string name, const geometry& geometryType, const material& materialType, vec position, vec orientation): 
                  name(name), geometryType(geometryType.clone()), materialType(materialType.clone()), position(position), orientation(orientation) {}

    object::object(const geometry& geometryType, const material& materialType, vec position, vec orientation): 
                  object("object_" + to_string(num_created), geometryType, materialType, position, orientation) {num_created++;}

    bool object::inside(const vec& v) const {
        double theta = atan2(orientation(1), orientation(0)) - M_PI/2;
        auto R = Eigen::Rotation2Dd(theta);
        vec rot_v = R.toRotationMatrix()*(v-position);
        return geometryType->inside(rot_v); 
    }

    void object::move(const vec& dr) {
        position += dr;
    }

    void object::rotate(quat rot) {
        //orientation = rot._transformVector(orientation);
    }

    void object::write() {
        string sub_group_name = geometryType->group_name();
        auto gObjects = outFile->create_or_open_group("objects");
        auto gSubObject = gObjects.create_or_open_group(sub_group_name);
        auto my_group = gSubObject.create_or_open_group(name);

        write_vec<double,h5cpp::dtype::Double>(my_group, position, "position");
        write_vec<double,h5cpp::dtype::Double>(my_group, orientation, "orientation");
        materialType->write(my_group);
        geometryType->write(my_group);
    }

    void object::set_owner(Field2D* F) {
        outFile = make_unique<h5cpp::h5file>(*F->outFile.get());
        write();
    }
}
