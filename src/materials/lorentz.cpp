#include "materials/lorentz.h"

using namespace std;

///Materials

namespace qbox {

    lorentz::lorentz(string name, double eps_inf, Array omega_0, Array delta_epsilon, Array gamma):
        material(name, "lorentz"), eps_inf(eps_inf), omega_0(omega_0), delta_epsilon(delta_epsilon), gamma(gamma) {};

    lorentz::lorentz(double eps_inf, Array omega_0, Array delta_epsilon, Array gamma):
        material("lorentz"), eps_inf(eps_inf), omega_0(omega_0), delta_epsilon(delta_epsilon), gamma(gamma) {};

    lorentz::lorentz(string name, double eps_inf, double omega_0_val, double delta_epsilon_val,
                         double gamma_val):
            lorentz(name, eps_inf, Array::Constant(1,omega_0_val), Array::Constant(1,delta_epsilon_val),
                    Array::Constant(1,gamma_val)) {};

    lorentz::lorentz(double eps_inf, double omega_0_val, double delta_epsilon_val,
                         double gamma_val):
            lorentz(eps_inf, Array::Constant(1,omega_0_val), Array::Constant(1,delta_epsilon_val),
                    Array::Constant(1,gamma_val)) {};

    unique_ptr<material> lorentz::clone() const {
        return unique_ptr<lorentz>(new lorentz(*this));
    }

    void lorentz::write(const h5cpp::h5file &outFile) const {
        auto group = get_group(outFile);

        auto dset = group.create_dataset("type", h5cpp::dtype::String, h5cpp::dspace(vector<hsize_t>{1}));
        dset.write(&group_name);
        dset = group.create_dataset("eps_inf", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{1}));
        dset.write(&eps_inf);
        dset = group.create_dataset("omega_0", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{Npoles()}));
        dset.write(omega_0.data());
        dset = group.create_dataset("delta_epsilon", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{Npoles()}));
        dset.write(delta_epsilon.data());
        dset = group.create_dataset("gamma", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{Npoles()}));
        dset.write(gamma.data());
    }

    Array lorentz::alpha(double dt) const {
        return (2 - omega_0.pow(2)*pow(dt,2))/(1 + gamma*dt);
    }

    Array lorentz::beta(double dt) const {
        return (gamma*dt - 1)/(gamma*dt + 1);
    }

    Array lorentz::delta(double dt) const {
        return (delta_epsilon*omega_0.pow(2)*pow(dt,2))/(1 + gamma*dt);
    }

    double lorentz::Ca_prev(double dt) const {
        double delta_sum = delta(dt).sum();
        return (delta_sum/2)/(2*eps_inf + delta_sum);
    }

    double lorentz::Ca(double dt) const {
        double delta_sum = delta(dt).sum();
        return (2*eps_inf)/(2*eps_inf + delta_sum/2);
    }

    double lorentz::Cb(double dt) const {
        double delta_sum = delta(dt).sum();
        return (2*dt)/(2*eps_inf + delta_sum/2);
    }

    double lorentz::Da(double dt) const {
        return 1;
    }

    double lorentz::Db(double dt) const {
        return dt;
    }

}
