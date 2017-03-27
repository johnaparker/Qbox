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
    //box_monitor m1("m1", volume({50,50}, {70,70}), 1/30.0,3/30.0, 500, false); 
    //box_monitor m1("m1",{50,50}, {70,70}, (freq), 100); 
    //surface_monitor m1("m1",{50,50}, {50,70},0,5*f, 100); 
    //surface_monitor m2(m1); 
    object o1(block({50,35}), simple_material(12), {70,50}, {1,1});
    //test.add_object(o1);

    box_monitor m1("m1", volume({60,60}, 20,20), freq_data(1/30.0,3/30.0, 500), false); 
    test.add_monitor(m1);
    box_monitor m2("m2", volume({90,90}, 20,20), freq_data(1/30.0,3/30.0, 500), false); 
    test.add_monitor(m2);

    gaussian_point_source s1({60,60}, f, 1/200.0, 80);
    //continuous_point_source s1({60,60}, 1/20.0);
    //gaussian_line_source s1(surface({30,0}, {30,120}), f, 1/200.0, 80);
    //continuous_line_source s1(surface({110,30}, {10,30}), f);
    test.add_source(s1);

    for (int i = 0; i != 4000; i++) {
        test.update();
        test.writeE();
    }
    m1.write_flux();
    m2.write_flux();
    //m1.write_flux_sides();
}
