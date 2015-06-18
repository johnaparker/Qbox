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
    double dx = 60e-2/50;
    double dt = dx/(2*c0);
    double L = 120;
    double tf = 1e-6;

    Field1D test(L,dx);
    test.run(tf,dt);

    ostringstream convert;
    convert << test.Nx;
    string sx = convert.str();
    string command = "python process.py " + sx; 
    //system(command.c_str());
}

