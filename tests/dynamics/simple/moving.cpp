#include "qbox.h"
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 7;
    int res = 2;
    double Lx = 100;
    double Ly = 100;
    double f = 2/30.0;
    vec center{Lx/2,Ly/2};

    grid_properties grid(Lx,Ly,res,pml_thickness);
    Field2D test(grid, "out.h5");
    test.set_tfsf(volume(center, 80), continuous_time(f));

    simple_material mat(5,1,10);
    dynamic_object obj(ellipse(6,12), mat, center, 0); 
    test.add_object(obj);

    double omega_1 = 0.03; 
    double omega_2 = -0.06; 
    double r = 20;
    
    for (int T = 0; T != 30; T++) {
        test.remove_monitors();         // Reset monitors
        test.clear_fields();           // Reset fields

        double x = r*cos(omega_1*T) + center(0);
        double y = r*sin(omega_1*T) + center(1);
        double theta = omega_2*T;
        obj.set_position(vec(x,y));
        obj.set_theta(theta);
        test.update_material_grid();

        volume_monitor<DFT::Ez> m1(grid.domain(), Array::Constant(1,f)); 

        for (int i = 0; i != 1300; i++) {
            test.update();
            if (i == 300)
                test.add_monitor(m1);
        }
        m1.write();
        obj.write_current();
        cout << "\nBig time step: " << T << endl;
    }
}
