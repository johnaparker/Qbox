#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 20;
    int res = 2;

    grid_properties grid(120,120,res,pml_thickness);
    grid.set_tfsf(12, 12);
    //grid.set_tfsf({12,12},{68,119});

    Field2D test(grid, "out.h5");
    
    double f = 2/30.0;
    box_monitor m1("m1",{50,50}, {70,70}, 1/30.0,3/30.0, 500, false); 
    //box_monitor m1("m1",{50,50}, {70,70}, (freq), 100); 
    //surface_monitor m1("m1",{50,50}, {50,70},0,5*f, 100); 
    //surface_monitor m2(m1); 
    block c1(vec(50,35));
    simple_material d1(12);
    object o1(c1, d1, vec(70,50), {1,1});

    test.add_object(o1);

    test.add_monitor(m1);

    gaussian_point_source s1({30,30}, f, 1/200.0, 80);
    //continuous_point_source s1({60,60}, 1/20.0);
    test.add_source(s1);

    for (int i = 0; i != 1000; i++) {
        test.update();
        test.writeE();
    }
    m1.write_flux();
    //m1.write_flux_sides();
}
