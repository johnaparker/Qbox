#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "field2.h"


//make a header file for every cc file
//make a universal resolution
//move towards specifying actual spatial values, not indices.

using namespace std;
double c = 3e8;

int main(int argc, char* argv[]) {
    double dx = 50e-2;
    double dt = dx/(2*c0);
    int pml_thickness = 12;

    grid_properties grid(200,200,dx,pml_thickness);
    //grid.set_tfsf(12, 12);
    //grid.set_tfsf({12,12},{68,119});


    Field2D test(grid,dt);
    
    rectangle r({30,0}, {40,80});
    r.set_eps(12);
    test.add_object(r);
    rectangle r2({30,70}, {200,80});
    r2.set_eps(12);
    test.add_object(r2);

    continuous_line_source s1({30,12},{40,12},c/(66.7*dx));
    test.add_source(s1);

    for (int i = 0; i != 1200; i++) {
        test.update();
        test.writeE("out.h5");
    }
}

