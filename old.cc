#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>


using namespace std;

int main(int argc,char* argv[]) {
    double mu = 1;
    const double c0 = 3e8;
    double dx = 60e-2/50;
    double dt = dx/(2*c0)*mu;
    double L = 120;
    double tf = 1e-6;
    cout << left;
    cout << setw(10) << "dt:" << dt << " s" << endl;
    cout << setw(10) << "dx:" << dx << " m" << endl;


    int KE =round(L/dx);
    double ex[KE], hy[KE], epsinv[KE];
    int n,k,kc,ke,NSTEPS;
    double T;
    double t0,spread,pulse;

    ofstream outE("Eout.txt");
    ofstream outH("Hout.txt");

    for (k=0; k < KE; ++k) {
        ex[k] = 0;
        hy[k] = 0;
        epsinv[k] = (k < KE/2) ? 1: 1/4.0;
    }
    double f = 10e6;

    kc = KE/2;
    t0 = 40.0;
    spread = 12;
    T = 0;

    //istringstream ss(argv[1]);
    //if (!(ss >> NSTEPS))
    //   cerr << "Invalid number " << argv[1] << '\n';

    int saveTime = 1000;
    int count = 0;
    NSTEPS = round(tf/dt);
    for (n=1; n <= NSTEPS; n++) {
        T += 1;
        for (k=1; k<KE; k++) {
            ex[k] = ex[k] + 0.5*mu*epsinv[k]*(hy[k-1]-hy[k]);
        }

        pulse = exp(-.5*(pow((t0-T)/spread,2.0)));
        pulse = sin(2*M_PI*f*dt*T);
        ex[0] = pulse;
        for (k=0; k<KE-1; k++) {
            hy[k] = hy[k] + .5*mu*(ex[k] - ex[k+1]);
        }
       // for (k=0; k<KE; k++) {
       //     outE << ex[k] << endl;
       //     outH << hy[k] << endl;
       // } 
        count++;
        if (count >= saveTime) {
            cout << n << "/" << NSTEPS << '\r';
            cout.flush();
            count = 0;
        }
    }
    cout << left;
    cout << setw(10) << "Total T:" << T*dt << " s" << endl;
    cout << setw(10) << "L:" << dx*KE << " m" << endl;
    cout << setw(10) << "Freq:" << f << " Hz" << endl;
    cout << setw(10) << "lamda:" << c0/f << " m" << endl;
    ostringstream convert;
    convert << KE;
    string sx = convert.str();
    string command = "python process.py " + sx; 
    
    outE.close();
    outH.close();
    //system(command.c_str());


}



