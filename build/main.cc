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


    Field2D test(grid);
    
    double f = 1/30.0;
    box_monitor m1("m1",{50,50}, {70,70}, 0,5*f, 100); 
    //box_monitor m1("m1",{50,50}, {70,70}, (freq), 100); 
    //surface_monitor m1("m1",{50,50}, {50,70},0,5*f, 100); 
    //surface_monitor m2(m1); 
    cylinder c1(60,60,5);
    c1.set_eps(4);
    test.add_object(c1);
    test.add_monitor(m1);

    gaussian_point_source s1({40,60}, 30, 3);
    //continuous_point_source s1({60,72}, 1/20);
    test.add_source(s1);
    //test.add_monitor(m1);

    for (int i = 0; i != 1000; i++) {
        test.update();
        //test.writeE("out.h5");
    }
    m1.write("out.h5", false);
    //m1.write_sides("out.h5", true);

}
