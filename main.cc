#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
#define KE 200

int main(int argc,char* argv[]) {
    float ex[KE], hy[KE];
    int n,k,kc,ke,NSTEPS;
    float T;
    float t0,spread,pulse;

    ofstream outE("Eout.txt");
    ofstream outH("Hout.txt");

    for (k=1; k < KE; ++k) {
        ex[k] = 0;
        hy[k] = 0;
    }

    kc = KE/2;
    t0 = 40.0;
    spread = 12;
    T = 0;

    istringstream ss(argv[1]);
    if (!(ss >> NSTEPS))
        cerr << "Invalid number " << argv[1] << '\n';

    for (n=1; n <= NSTEPS; n++) {
        T += 1;
        for (k=1; k<KE; k++) {
            ex[k] = ex[k] + 0.5*(hy[k-1]-hy[k]);
        }

        pulse = exp(-.5*(pow((t0-T)/spread,2.0)));
        ex[kc] = pulse;
        for (k=0; k<KE-1; k++) {
            hy[k] = hy[k] + .5*(ex[k] - ex[k+1]);
        }
        for (k=1; k<=KE; k++) {
            outE << ex[k] << endl;
            outH << hy[k] << endl;
        }
        cout << n << "/" << NSTEPS << endl;
    }
    ostringstream convert;
    convert << NSTEPS;
    string st = convert.str();
    convert.str("");
    convert << KE;
    string sx = convert.str();
    string command = "python process.py " + st + " " + sx; 
    cout << command << endl;
    
    outE.close();
    outH.close();
    system(command.c_str());


}



