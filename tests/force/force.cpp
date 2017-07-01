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
    auto& o1 = scat.add<object>(cylinder(20), simple_material(2), vec(60,60));

    auto& box = o1.get_box_monitor(freq_data);

    for (int i = 0; i != 8000; i++) {
        scat.update();
    }

    tfsf.flux().write();
    box.force().write();
    box.torque().write();
}
