#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "field.h"

using namespace std;

int main(int argc, char* argv[]) {
    double dx = 10e-2;
    double dt = dx/(2*c0);
    int Nx = 200;
    double tf = 7e-7;

    Field1D test(Nx,dx,dt);
    test.run(tf);

    ostringstream convert;
    convert << test.Nx;
    string sx = convert.str();
    string command = "python process.py " + sx; 
    system(command.c_str());
}

