#include <qbox.h>
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
    

    simple_material mat(5,1,10);
    //simple_material mat(5);
    vec orient(1,1);
    auto geo = block(vec(10,20));
    dynamic_object obj1(geo, center - vec(20,0), 0); 
    test.add_object(obj1, mat);

    dynamic_object obj2(geo, center + vec(20,0), 0); 
    test.add_object(obj2, mat);


    double omega_1 = 0.03; 
    double omega_2 = -0.06; 
    double r = 20;
    
    for (int T = 0; T != 300; T++) {
        test.clear_monitors();
        test.clear_fields();
        test.set_tfsf(volume(center, 80), continuous_time(f));
        double x1 = r*cos(omega_1*T) + center(0);
        double x2 = -r*cos(omega_1*T) + center(0);
        double y1 = r*sin(omega_1*T) + center(1);
        double y2 = -r*sin(omega_1*T) + center(1);
        double theta = omega_2*T;
        obj1.set_position(vec(x1,y1));
        obj1.set_theta(theta);
        obj2.set_position(vec(x2,y2));
        obj2.set_theta(theta);
        test.update_material_grid(mat);

        volume_monitor m1(volume(vec(0,0), vec(grid.Lx, grid.Ly)), freq_data(f)); 

        for (int i = 0; i != 1300; i++) {
            test.update();
            if (i == 300)
                test.add_monitor(m1);

            //test.writeE();
        }
        m1.write_dft();
    }
}
