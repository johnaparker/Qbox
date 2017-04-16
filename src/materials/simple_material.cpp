#include "materials/simple_material.h"

using namespace std;

///Materials

namespace qbox {

    simple_material::simple_material(double eps, double mu, double conduc): eps(eps), mu(mu), conduc(conduc) {};

    unique_ptr<material> simple_material::clone() const {
        return unique_ptr<simple_material>(new simple_material(*this));
    }

    void simple_material::write(const h5cpp::h5group &group) {
        auto dspace = h5cpp::dspace(vector<hsize_t>{1});

        auto dset = group.create_dataset("eps", h5cpp::dtype::Double, dspace);
        dset.write(&eps);

        dset = group.create_dataset("mu", h5cpp::dtype::Double, dspace);
        dset.write(&mu);

        dset = group.create_dataset("conduc", h5cpp::dtype::Double, dspace);
        dset.write(&conduc);
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
