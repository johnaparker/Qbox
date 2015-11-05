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
double c = 3e8;

int main(int argc, char* argv[]) {
    double dx = 50e-2;
    double dt = dx/(2*c0);
    int pml_thickness = 12;

    grid_properties grid(120,120,dx,pml_thickness);
    //grid.set_tfsf(12, 12);
    //grid.set_tfsf({12,12},{68,119});


    Field2D test(grid,dt);
    
    
    double f = c/(66.7*dx);
    box_monitor m1("m1",{30,30}, {90,90}, 0, 5*f, 100); 
    test.add_monitor(m1);

    gaussian_point_source s1(60, 95, 1e-7, 1e-8);
    test.add_source(s1);

    for (int i = 0; i != 4200; i++) {
        test.update();
        test.writeE("out.h5");
    }
    m1.write("out.h5");
    m1.write_sides("out.h5");
}

