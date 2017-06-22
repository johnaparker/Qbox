#include "object.h"
#include <eigen3/Eigen/Geometry>
#include "math.h"
#include "field2.h"
#include "materials/material.h"

#include "monitors/box_monitor.h"

using namespace std;

namespace qbox {

    int object::num_created = 0;

    object::object(string name, const geometry& geometryType, const material_variant &mat, vec position, double theta):
                  name(name), geometryType(geometryType.clone()), mat(mat), position(position), theta(theta) {}

    object::object(const geometry& geometryType, const material_variant &mat, vec position, double theta):
                  object("object_" + to_string(num_created), geometryType, mat, position, theta) {num_created++;}

    bool object::inside(const vec& v) const {
        auto R = Eigen::Rotation2Dd(theta).inverse();
        auto T = Eigen::Translation<double,2>(position).inverse();
        auto tr = R*T;
        vec transformed_v = tr*v;
        return geometryType->inside(transformed_v); 
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

        h5cpp::write_vector<double>(position, my_group, "position");
        h5cpp::write_scalar(theta, my_group, "theta");
        geometryType->write(my_group);
        write_material();
    }

    void object::write_material() const {
        auto my_group = get_group();

        auto group_path = visit([this](auto&& arg)->string{return arg.get_group_path();}, mat);
        auto mat_reference = outFile->create_reference(group_path); 
        h5cpp::write_scalar(mat_reference, my_group, "material");

        //auto mat_group = mat->get_group(*outFile);
        //auto mat_name = mat->get_name();
        //dset.write(&mat_name);
    }

    box_monitor<DFT::all> object::get_box_monitor(const Array& freq, double a) {
        //*** name monitor 'object_name'_monitor?
        //*** write a h5ref in object to monitor (requires set_owner to be called first...)
        auto tight_box = get_bounding_box().value();
        volume box(tight_box.a - vec(a,a), tight_box.b + vec(a,a));
        return box_monitor<DFT::all>(box, freq); 
    }

    void object::set_owner(Field2D* F) {
        outFile = make_unique<h5cpp::h5file>(*F->outFile.get());
        write();
    }

    std::optional<volume> object::get_bounding_box() const {
        auto box = geometryType->get_bounding_box();
        if (box) {
            Eigen::Matrix2d R;
            R << cos(theta), sin(theta),
                 sin(theta), cos(theta);
            R = R.array().abs();
            
            vec new_dim = R*box->dim;
            box = volume(position - new_dim/2, position + new_dim/2);
        }

        return box;
    }
}
