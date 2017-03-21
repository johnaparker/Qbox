#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 20;
    int res = 2;
    double f = 2/30.0;

    grid_properties grid(120,120,res,pml_thickness);
{
    Field2D norm(grid, "norm.h5");

    box_monitor m1("m_norm",{50,50}, {70,70}, 1/30.0,3/30.0, 500); 
    norm.add_monitor(m1);

    gaussian_point_source s1({30,60}, f, 1/200.0, 80);
    norm.add_source(s1);

    for (int i = 0; i != 1000; i++) {
        norm.update();
        norm.writeE();
    }
    m1.write_flux();
    m1.write_flux_sides();
}


    Field2D scat(grid, "scat.h5");

    cylinder c1(5);
    simple_material d1(4);
    object o1(c1, d1, vec(60,60));
    scat.add_object(o1);

    box_monitor m2("m_scat",{50,50}, {70,70}, 1/30.0,3/30.0, 500); 
    scat.add_monitor(m2);

    gaussian_point_source s2({30,60}, f, 1/200.0, 80);
    scat.add_source(s2);

    for (int i = 0; i != 1000; i++) {
        scat.update();
        scat.writeE();
    }
    m2.write_flux();
}
