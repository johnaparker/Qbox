#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 20;
    int res = 2;
    double f = 2/30.0;

    grid_properties grid(120,120,res,pml_thickness);

    Field2D norm(grid, "inc.h5");

    box_monitor box_inc("box_inc",volume({60,60}, 40), freq_data(1/30.0,3/30.0, 200)); 
    surface_monitor inc("inc",surface({40,60}, {80,60}), freq_data(1/30.0,3/30.0, 200)); 
    norm.add_monitor(box_inc);
    norm.add_monitor(inc);

    gaussian_line_source s1(fields::Ez, surface({0,30},{120,30}), f, 1/200.0, 80);
    norm.add_source(s1);

    for (int i = 0; i != 4000; i++) {
        norm.update();
        //norm.writeE();
    }
    box_inc.write_flux();
    inc.write_flux();


    Field2D scat(grid, "scat.h5");

    cylinder c1(20);
    simple_material d1(2);
    object o1(c1, d1, vec(60,60));
    scat.add_object(o1);

    box_monitor box_scat("box_scat",volume({60,60}, 40), freq_data(1/30.0,3/30.0, 200)); 
    scat.add_monitor(box_scat);
    box_scat -= box_inc;

    gaussian_line_source s2(fields::Ez, surface({0,30},{120,30}), f, 1/200.0, 80);
    scat.add_source(s2);

    for (int i = 0; i != 4000; i++) {
        scat.update();
        //scat.writeE();
    }
    box_scat.write_flux();
}
