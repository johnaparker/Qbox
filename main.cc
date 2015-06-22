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
    double dx = 60e-2;
    double dt = dx/(2*c0);
    double dim[] = {60,60};
    double tf = 1e-7;

    Field2D test(dim,dx,dt);
    test.run(tf);

    ostringstream convert;
    convert << test.Nx;
    string sx = convert.str();
    convert.str("");
    convert.clear();
    convert << test.Ny;
    string sy = convert.str();
    string command = "python process.py " + sx + " " + sy; 
    system(command.c_str());
}

