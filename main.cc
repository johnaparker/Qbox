#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "field2.h"

using namespace std;

int main(int argc, char* argv[]) {
    double dx = 50e-2;
    double dt = dx/(2*c0);
    int pml_thickness = 12;

    grid_properties grid(50,130,dx,pml_thickness);
    grid.set_tfsf(18,38);

    double tf = 600*dt;

    Field2D test(grid,dt);
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

