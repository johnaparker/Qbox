#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "field2.h"
#include "monitors/surface_monitor.h"

using namespace std;


namespace qbox {
    surface_monitor::surface_monitor(string name, const surface &surf, const freq_data &freq, bool extendable): monitor(name,"surface_monitor",freq,extendable), surf(surf), dir(0), length(0) {
        F = nullptr;
    }
    surface_monitor::surface_monitor(const surface &surf, const freq_data &freq, bool extendable): monitor("surface_monitor",freq,extendable), surf(surf), dir(0), length(0) {
        F = nullptr;
    }

    void surface_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);
        auto isurf = newF->grid.to_grid(surf);
        length = isurf.dim.norm();

        prevE = Array::Zero(length);
        rE = tensor(length, freq.size());
        iE = tensor(length, freq.size());
        rH = tensor(length, freq.size());
        iH = tensor(length, freq.size());

        auto group = get_group();
        surf.write(group);
    }

    void surface_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)
        auto isurf = F->grid.to_grid(surf);
        auto *Hfield = isurf.dim[0] == 0 ? &F->Hy : &F->Hx;

        double E = 0;
        double H = 0;
        freq.update(F->t);

        for (int i = 0; i < length; i++) {
            ivec p = isurf.a + i*isurf.tangent;
            //H = ((*Hfield)(p)+ (*Hfield)(p - isurf.normal)
                    //+ (*Hfield)(p + isurf.tangent) + (*Hfield)(p - isurf.normal + isurf.tangent))/4;
            //E = (F->Ez(p) + F->Ez(p + isurf.tangent)
                    //+ prevE(i) + prevE(i+1))/4;
                    
            H = ((*Hfield)(p) + (*Hfield)(p - isurf.normal))/2;
            E = (F->Ez(p) + prevE(i))/2;
            prevE(i) = F->Ez(p);

            //E = (F->Ez(p));
            //H = ((*Hfield)(p));

            //H = isurf.normal[0] == 0 ? F->to_xgrid(fields::Hx, p) : F->to_ygrid(fields::Hy, p); 
            //E = isurf.normal[0] == 0 ? F->to_xgrid(fields::Ez, p) : F->to_ygrid(fields::Ez, p); 

            //vec pr = F->grid.to_real(p) + surf.tangent*F->dx/2.0;
            //H = isurf.normal[0] == 0 ? F->interpolate(fields::Hx, pr) : F->interpolate(fields::Hy, pr);
            //E = F->interpolate(fields::Ez, pr);
            //double tempE = E;
            //E = (E + prevE(i))/2;
            //prevE(i) = tempE;

            const int max_freq = freq.size();
#pragma GCC ivdep
            for (int j = 0; j < max_freq; j++) {
                rE(i,j) += E*freq.get_cosf(j);
                iE(i,j) += E*freq.get_sinf(j);
                rH(i,j) += H*freq.get_cosf(j);
                iH(i,j) += H*freq.get_sinf(j);
            }
        }
        //prevE(length) = F->Ez(isurf.b);
    }

    //*** rename to flux
    Array surface_monitor::compute_flux() const {
        Array S = Array::Zero(freq.size());

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < freq.size(); j++) {
                S[j] += rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            }
        }
        S *= F->dx;
        return S;
    }

    ComplexArray surface_monitor::ntff(const vec &center, const vec &pc) const {
        vec p = pc - center;
        double r = p.norm();
        Array omega = 2*M_PI*freq.get_freq();
        Array k = omega;
        ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
        ComplexArray integral = ComplexArray::Zero(freq.size());
        

        vec tangent = surf.tangent;
        vec normal = surf.normal;  //*** this needs to be outward normal

        double Jsgn = tangent.dot(vec(1,1));
        double Msgn = tangent.dot(vec(-1,1));

        for (int i = 0; i < length; i++) {
            vec p_prime = surf.a + tangent*i*F->dx - center; //*** account for yee grid half-step here
            double angle = p.dot(normal)/r;  //*** worry about the sign of tangent here
            for (int j = 0; j < freq.size(); j++) {
                auto E = rE(i,j) + 1i*iE(i,j);
                auto H = rH(i,j) + 1i*iH(i,j);

                auto Jeq_term = Jsgn*omega[j]*H;
                auto Meq_term = Msgn*k[j]*E*angle;
                auto integand = (Jeq_term + Meq_term)*exp(1i*k[j]*p.dot(p_prime)/r);
                integral[j] += integand; 
            }
        }

        return factor*integral;
    }

}
