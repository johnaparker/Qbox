#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 5;
    double res = 2;
    double f = 2/30.0;
    auto dft = freq_data(1/30.0,3/30.0, 200);

    grid_properties grid(120,120,res,pml_thickness);
    Field2D scat(grid, "scat.h5");

    scat.set_tfsf(volume({60,60}, 65), gaussian_time(f, 1/200.0, 80));
    scat.set_tfsf_freq(dft);

    object o1(cylinder(20), simple_material(2), vec(60,60), vec(1,1));
    scat.add_object(o1);

    cylinder_monitor box_scat("box_scat", cylinder_surface(vec(60,60), 50), dft); 
    //box_monitor box_scat("box_scat",volume({60,60}, 75), dft); 
    scat.add_monitor(box_scat);

    //line_source s2(fields::Ez, surface({0,30},{120,30}), gaussian_time(f, 1/200.0, 80));
    //scat.add_source(s2);

    for (int i = 0; i != 8000; i++) {
        scat.update();
        //scat.writeE();
    }
    box_scat.write_flux();
    scat.write_tfsf();
}
