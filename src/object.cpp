#include "object.h"
#include <eigen3/Eigen/Geometry>
#include "math.h"
#include "field2.h"
#include "materials/material.h"

using namespace std;

namespace qbox {

    int object::num_created = 0;

    object::object(string name, const geometry& geometryType, vec position, vec orientation): 
                  name(name), geometryType(geometryType.clone()), position(position), orientation(orientation) {}

    object::object(const geometry& geometryType, vec position, vec orientation): 
                  object("object_" + to_string(num_created), geometryType, position, orientation) {num_created++;}

    bool object::inside(const vec& v) const {
        double theta = atan2(orientation(1), orientation(0)) - M_PI/2;
        auto R = Eigen::Rotation2Dd(theta).inverse();
        auto T = Eigen::Translation<double,2>(position).inverse();
        auto tr = R*T;
        vec transformed_v = tr*v;
        return geometryType->inside(transformed_v); 
    }

    void object::move(const vec& dr) {
        position += dr;
    }

    void object::rotate(quat rot) {
        //orientation = rot._transformVector(orientation);
    }

    h5cpp::h5group object::get_group() const {
        string sub_group_name = geometryType->group_name();
        auto gObjects = outFile->create_or_open_group("objects");
        auto gSubObject = gObjects.create_or_open_group(sub_group_name);
        auto my_group = gSubObject.create_or_open_group(name);
        return my_group;
    }

    void object::write() const {
        auto my_group = get_group();

        write_vec<double,h5cpp::dtype::Double>(my_group, position, "position");
        write_vec<double,h5cpp::dtype::Double>(my_group, orientation, "orientation");
        geometryType->write(my_group);
    }

    void object::write_material(const material* mat) const {
        auto my_group = get_group();

        auto dset = my_group.create_dataset("material", h5cpp::dtype::Reference, h5cpp::dspace(vector<hsize_t>{1}));
        auto mat_reference = outFile->create_reference(mat->get_group_path()); 
        dset.write(&mat_reference);

        //auto mat_group = mat->get_group(*outFile);
        //auto mat_name = mat->get_name();
        //dset.write(&mat_name);

    }

    void object::set_owner(Field2D* F) {
        outFile = make_unique<h5cpp::h5file>(*F->outFile.get());
        write();
    }
}
