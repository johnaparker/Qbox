#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "field2.h"

using namespace std;
double c = 3e8;

int main(int argc, char* argv[]) {
    double dx = 50e-2;
    double dt = dx/(2*c0);
    int pml_thickness = 12;

    grid_properties grid(80,120,dx,pml_thickness);
    //grid.set_tfsf(12, 12);
    //grid.set_tfsf({12,12},{68,119});

    double tf = 600*dt;

    Field2D test(grid,dt);
    
    rectangle r({30,0}, {40,80});
    r.set_eps(10);
    test.add_object(r);
    rectangle r2({30,70}, {80,80});
    r2.set_eps(10);
    test.add_object(r2);

    continuous_point_source s1(35,30,c/(15*dx));
    test.add_source(s1);
    test.run(tf);

    ostringstream convert;
    convert << test.Nx;
    string sx = convert.str();
    convert.str("");
    convert.clear();
    convert << test.Ny;
    string sy = convert.str();
    string command = "python process.py " + sx + " " + sy; 
    //system(command.c_str());
}

