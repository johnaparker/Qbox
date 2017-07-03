#include <qbox.h>
#include <iostream>
#include "material.h"

using namespace std;
using namespace qbox;

const double Lx = 120;
const double Ly = 120;

int main() {
    int pml_thickness = 8;
    double res = 2;
    double f = 2/30.0;
    auto dft = Array::LinSpaced(200, 1/30.0,3/30.0);

    grid_properties grid(Lx,Ly,res,pml_thickness);
    Field2D scat(grid, "scat.h5");

    Array omega_0_vals(1), gamma_vals(1);
    omega_0_vals << omega_0;
    gamma_vals << gamma_0;
    drude mat(2,omega_0_vals, gamma_vals);

    auto& tfsf = scat.set_tfsf(volume({Lx/2,Ly/2}, 75), gaussian_time(f, 1/100.0, 200).freq(dft));
    //auto& m_scat = scat.add<cylinder_monitor<DFT::tangent>> ("scat", cylinder_surface(vec(60,60), 50), dft); 
    auto& m_scat = scat.add<box_monitor<DFT::tangent>> ("scat", volume(vec(Lx/2,Ly/2), 80), dft); 
    scat.add<object> (cylinder(20), mat, vec(Lx/2,Ly/2));

    for (int i = 0; i != 8000; i++) {
        scat.update();
    }

    m_scat.flux().write();
    tfsf.flux().write();

}
