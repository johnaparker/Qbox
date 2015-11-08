#include "field2.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>


//make a header file for every cc file
//make a universal resolution
//move towards specifying actual spatial values, not indices.

using namespace std;

int main(int argc, char* argv[]) {
    int pml_thickness = 12;
    int res = 2;

    grid_properties grid(120,120,res,pml_thickness);
    //grid.set_tfsf(12, 12);
    //grid.set_tfsf({12,12},{68,119});


    Field2D test(grid);
    
    double f = 1/30.0;
    box_monitor m1("m1",{50,50}, {70,70}, 0, 5*f, 100); 
    cylinder c1(60,60,5);
    c1.set_eps(4);
    test.add_object(c1);
    test.add_monitor(m1);

    gaussian_point_source s1({40,60}, 30, 3);
    //continuous_point_source s1({60,72}, 1/20);
    test.add_source(s1);

    for (int i = 0; i != 7000; i++) {
        test.update();
        //test.writeE("out.h5");
    }
    m1.write("out.h5", true);
    m1.write_sides("out.h5", true);
}

