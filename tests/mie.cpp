#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 5;
    double res = 2;
    double f = 2/30.0;
    auto freq_data = Array::LinSpaced(200, 1/30.0,3/30.0);

    grid_properties grid(120,120,res,pml_thickness);
    Field2D scat(grid, "scat.h5");

    scat.set_tfsf(volume({60,60}, 65), gaussian_time(f, 1/100.0, 80));
    //scat.set_tfsf(volume({60,60}, 65), continuous_time(1/28.0));

    object o1(cylinder(20), debye(2,1,1), vec(60,60));
    //scat.add_object(o1, simple_material(2));
    scat.add_object(o1);

    cylinder_monitor<DFT::tangent> box_scat("box_scat", cylinder_surface(vec(60,60), 50), freq_data); 
    //box_monitor box_scat("box_scat",volume({60,60}, 75), freq_data); 
    scat.add_monitor(box_scat);

    //line_source s2(fields::Ez, surface({0,30},{120,30}), gaussian_time(f, 1/200.0, 80));
    //scat.add_source(s2);

    for (int i = 0; i != 2000; i++) {
        scat.update();
        // scat.writeE();
    }
    // box_scat.flux().write();
    // box_scat.write();
    // scat.write_tfsf();
}
