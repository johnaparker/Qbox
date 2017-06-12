#include "materials/simple_material.h"

using namespace std;

///Materials

namespace qbox {

    simple_material::simple_material(string name, double eps, double mu, double conduc): material(name, "simple"), 
                         eps(eps), mu(mu), conduc(conduc) {};
    simple_material::simple_material(double eps, double mu, double conduc): material("simple"), eps(eps), mu(mu), conduc(conduc) {};

    unique_ptr<material> simple_material::clone() const {
        return unique_ptr<simple_material>(new simple_material(*this));
    }

    void simple_material::write(const h5cpp::h5file &outFile) const {
        auto group = get_group(outFile);

        h5cpp::write_scalar(group_name, group, "material_type");
        h5cpp::write_scalar(eps, group, "eps");
        h5cpp::write_scalar(mu, group, "mu");
        h5cpp::write_scalar(conduc, group, "conduc");
    }

    double simple_material::Ca(double dt) const {
        return (1 - conduc*dt/(2*eps))/(1 + conduc*dt/(2*eps));
    }

    double simple_material::Cb(double dt) const {
        return (dt/(eps))/(1 + conduc*dt/(2*eps));
    }

    double simple_material::Da(double dt) const {
        return 1;
    }

    double simple_material::Db(double dt) const {
        return dt;
    }
}
