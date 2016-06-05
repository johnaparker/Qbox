#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "h5out.h"
#include "surface_monitor.h"

using namespace std;


namespace apine {
    surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, shared_ptr<freq_data> freq, int N): monitor(name,freq,N), p1(p1), p2(p2), dir(0), length(0) {
        F = nullptr;
        prevE = nullptr;
    }

    surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double fmin, double fmax, int N): surface_monitor(name, p1, p2, nullptr, N) {
        freq = shared_ptr<freq_data> (new freq_data(fmin, fmax, N));
    }

    surface_monitor::surface_monitor(string name, vector<int> p1, vector<int> p2, double f): 
            surface_monitor(name, p1, p2, nullptr, 1) {
        freq = shared_ptr<freq_data> (new freq_data(f));
    }

    void surface_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);
        p1g = (F->grid).convertToGrid(p1);
        p2g = (F->grid).convertToGrid(p2);
        dir = get_direction(p1g, p2g);
        length = p2g[dir] - p1g[dir];
        prevE = unique_ptr<double[]>(new double[length+1]);
        rE = matrix<double>(length, N);
        iE = matrix<double>(length, N);
        rH = matrix<double>(length, N);
        iH = matrix<double>(length, N);
        for (int i = 0; i != length; i++) {
            prevE[i] = 0;
            for (int j = 0; j!= N; j++) {
                rE[i][j] = 0;
                iE[i][j] = 0;
                rH[i][j] = 0;
                iH[i][j] = 0;
            }
        }
        prevE[length] = 0;
    }

    void surface_monitor::update() {
        matrix<double> *Hfield;
        if (dir == 0)
            Hfield = &F->Hx;
        if (dir == 1)
            Hfield = &F->Hy;

        int a = p1g[0];
        int b = p1g[1];
        double E, H;
        (*freq).update(F->t);

        //this if check could be done outside the for loop somehow
        for (int i = 0; i != length; i++) {
            if (dir == 0) {
                a = p1g[0] + i;
                H = ((*Hfield)[a][b] + (*Hfield)[a][b-1]
                        + (*Hfield)[a+1][b] + (*Hfield)[a+1][b-1])/4;
                E = (F->Ez[a][b] + F->Ez[a+1][b]
                        + prevE[i] + prevE[i+1])/4;
            }
            else if (dir == 1) {
                b = p1g[1] + i;
                H = ((*Hfield)[a][b] + (*Hfield)[a-1][b]
                        + (*Hfield)[a][b+1] + (*Hfield)[a-1][b+1])/4;
                E = (F->Ez[a][b] + F->Ez[a][b+1]
                        + prevE[i] + prevE[i+1])/4;
            }
            prevE[i] = F->Ez[a][b];

            for (int j = 0; j != N; j++) {
                rE[i][j] += E*(*freq).get_cosf(j);
                iE[i][j] += E*(*freq).get_sinf(j);
                rH[i][j] += H*(*freq).get_cosf(j);
                iH[i][j] += H*(*freq).get_sinf(j);
            }
        }
        if (dir == 0)
            a += 1;
        else
            b += 1;
        prevE[length] = F->Ez[a][b];
    }

    void surface_monitor::compute_flux(double *S) {
        for (int i = 0; i != N; i++) 
            S[i] = 0;

        for (int j = 0; j != N; j++) {
            for (int i = 0; i != length; i++) {
                S[j] += rE[i][j]*rH[i][j] + iE[i][j]*iH[i][j];
            }
            S[j] *= F->dx;
        }
    }

    void surface_monitor::write(string filename, bool extendable) {
        double *S = new double[N];
        compute_flux(S);
        F->write_monitor(filename, name, S, N, extendable); 
        delete[] S;
    }
}


