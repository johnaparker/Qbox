#include "dynamic_object.h"
#include <eigen3/Eigen/Geometry>
#include "math.h"
#include "field2.h"
#include "materials/material.h"

using namespace std;

namespace qbox {

    dynamic_object::dynamic_object(string name, const geometry& geometryType, const material_variant& mat, vec position, double theta): 
                  object(name, geometryType, mat, position, theta) {}

    dynamic_object::dynamic_object(const geometry& geometryType, const material_variant& mat, vec position, double theta): 
                  object(geometryType, mat, position, theta) {}

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

        h5cpp::write_vector<double>(position, my_group, "position", h5cpp::append::True);
        h5cpp::write_scalar(theta, my_group, "theta", h5cpp::append::True);
        geometryType->write(my_group);
        write_material();
    }

    void dynamic_object::write_current() const {
        auto my_group = get_group();

        auto dset = my_group.open_dataset("position");
        dset.append(position.data());
        dset = my_group.open_dataset("theta");
        dset.append(&theta);
    }
}
