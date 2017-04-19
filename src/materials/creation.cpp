#include "math.h"
#include "materials/creation.h"

using namespace std;

namespace qbox {

    lorentz drude_lorentz(const drude& drude_material, const lorentz& lorentz_material) {
        int Npoles = drude_material.Npoles() + lorentz_material.Npoles();
        double eps_inf = drude_material.get_eps_inf();
        Array gamma(Npoles), omega_0(Npoles), delta_epsilon(Npoles);

        double small = 1e-20;
        Array omega_0_drude = Array::Constant(drude_material.Npoles(), small);
        Array delta_epsilon_drude = drude_material.get_omega_0().pow(2)/pow(small,2);
        Array gamma_drude = drude_material.get_gamma()/2;

        omega_0 << omega_0_drude, lorentz_material.get_omega_0();
        delta_epsilon << delta_epsilon_drude, lorentz_material.get_delta_epsilon();
        gamma << gamma_drude, lorentz_material.get_gamma();

        return lorentz(eps_inf, omega_0, delta_epsilon, gamma);

    }
}
