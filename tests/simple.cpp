#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 12;
    int res = 2;

    grid_properties grid(120,120,res,pml_thickness);
    //grid.set_tfsf(12, 12);
    //grid.set_tfsf({12,12},{68,119});


    Field2D test(grid, "out.h5");
    
    double f = 1/30.0;
    box_monitor m1("m1",volume({50,50}, {70,70}), freq_data(0,5*f, 100)); 
    //box_monitor m1("m1",{50,50}, {70,70}, (freq), 100); 
    //surface_monitor m1("m1",{50,50}, {50,70},0,5*f, 100); 
    //surface_monitor m2(m1); 
    cylinder c1(5);
    simple_material d1(4);
    object o1(c1, d1, vec(60,60));

    test.add_object(o1);
    test.add_monitor(m1);

    gaussian_point_source s1({40,60}, f, 30, 3);
    //continuous_point_source s1({60,72}, 1/20);
    test.add_source(s1);

    for (int i = 0; i != 1000; i++) {
        test.update();
        //test.writeE("out.h5");
    }
    m1.write_flux();
    m1.write_flux_sides();

}
