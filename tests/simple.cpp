#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 20;
    int res = 2;

    grid_properties grid(120,120,res,pml_thickness);
    grid.set_tfsf(12, 12);

    Field2D test(grid, "out.h5");
    
    double f = 2/30.0;
    box_monitor m1("m1", volume({60,60}, 20,20), freq_data(1/30.0,3/30.0, 500), false); 
    test.add_monitor(m1);
    box_monitor m2("m2", volume({90,90}, 20,20), freq_data(1/30.0,3/30.0, 500), false); 
    test.add_monitor(m2);

    gaussian_point_source s1({60,60}, f, 1/200.0, 80);
    test.add_source(s1);

    for (int i = 0; i != 4000; i++) {
        test.update();
        //test.writeE();
    }
    m1.write_flux();
    m2.write_flux();
}
