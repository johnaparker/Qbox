#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 8;
    double res = 2;
    double f = 2/30.0;
    auto freq_data = Array::LinSpaced(200, 1/30.0,3/30.0);

    grid_properties grid(120,120,res,pml_thickness);
    Field2D scat(grid, "scat.h5");

    auto& tfsf = scat.set_tfsf(volume({60,60}, 65), gaussian_time(f, 1/100.0, 80).freq(freq_data));
    //scat.set_tfsf(volume({60,60}, 65), continuous_time(1/28.0));

    object o1(cylinder(20), simple_material(2), vec(60,60));
    //scat.add_object(o1, simple_material(2));
    scat.add_object(o1);

    auto box = o1.get_box_monitor(freq_data, grid.dx);
    scat.add_monitor(box);


    //line_source s2(fields::Ez, surface({0,30},{120,30}), gaussian_time(f, 1/200.0, 80));
    //scat.add_source(s2);

    for (int i = 0; i != 8000; i++) {
        scat.update();
        // scat.writeE();
    }
    tfsf.flux().write();
    box.force().write();
    box.torque().write();
}
