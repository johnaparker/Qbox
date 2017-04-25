#include "dynamic_object.h"
#include <eigen3/Eigen/Geometry>
#include "math.h"
#include "field2.h"
#include "materials/material.h"

using namespace std;

namespace qbox {

    int dynamic_object::num_created = 0;

    dynamic_object::dynamic_object(string name, const geometry& geometryType, vec position, double theta): 
                  name(name), geometryType(geometryType.clone()), position(position), theta(theta) {}

    dynamic_object::dynamic_object(const geometry& geometryType, vec position, double theta): 
                  dynamic_object("object_" + to_string(num_created), geometryType, position, theta) {num_created++;}

    bool dynamic_object::inside(const vec& v) const {
        auto R = Eigen::Rotation2Dd(theta).inverse();
        auto T = Eigen::Translation<double,2>(position).inverse();
        auto tr = R*T;
        vec transformed_v = tr*v;
        return geometryType->inside(transformed_v); 
    }

    void dynamic_object::move(const vec& dr) {
        position += dr;
    }

    void dynamic_object::rotate(double dtheta) {
        theta += dtheta;
    }

    void dynamic_object::set_position(const vec& p) {
        position = p;
    }

    void dynamic_object::set_theta(double new_theta) {
        theta = new_theta;
    }

    h5cpp::h5group dynamic_object::get_group() const {
        string sub_group_name = geometryType->group_name();
        auto gObjects = outFile->create_or_open_group("dynamic_objects");
        auto gSubObject = gObjects.create_or_open_group(sub_group_name);
        auto my_group = gSubObject.create_or_open_group(name);
        return my_group;
    }

    void dynamic_object::write() const {
        auto my_group = get_group();

        write_vec<double,h5cpp::dtype::Double>(my_group, position, "position");
        //write_vec<double,h5cpp::dtype::Double>(my_group, orientation, "orientation");
        geometryType->write(my_group);
    }

    void dynamic_object::write_material(const material* mat) const {
        auto my_group = get_group();

        auto dset = my_group.create_dataset("material", h5cpp::dtype::Reference);
        auto mat_reference = outFile->create_reference(mat->get_group_path()); 
        dset.write(&mat_reference);

        //auto mat_group = mat->get_group(*outFile);
        //auto mat_name = mat->get_name();
        //dset.write(&mat_name);

    }

    void dynamic_object::set_owner(Field2D* F) {
        outFile = make_unique<h5cpp::h5file>(*F->outFile.get());
        write();
    }
}
