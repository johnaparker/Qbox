#ifndef GUARD_surface_monitor_h
#define GUARD_surface_monitor_h

#include "monitor_types.h"
#include <vector>
#include <math.h>
#include "field2.h"

namespace qbox {

    //*** Perhaps allow a res parameter to "skip" some grid cells 
    //surface monitor: monitors all points inside a plane

    class surface_monitor: public open_monitor {
        static constexpr char* sub_group = "surface";
    public:
        surface_monitor(std::string name, const surface &surf, const Array &freq): rank_monitor(name, "surface"), surf(surf), freq(freq),  length(0) {
            F = nullptr;
        }
        surface_monitor(const surface &surf, const Array &freq): rank_monitor("surface"), surf(surf), freq(freq), length(0) {
            F = nullptr;
        }

        void set_F(Field2D *newF) {
            monitor::set_F(newF);
            auto isurf = newF->grid.to_grid(surf);
            length = isurf.dim.norm();

            prevE = Array::Zero(length);
            auto group = get_group();
            surf.write(group);
        }

        void update() {
            //*** this dir, Hfield combo is bad design (maybe use enum)
            auto isurf = F->grid.to_grid(surf);
            auto *Hfield = isurf.dim[0] == 0 ? &F->Hy : &F->Hx;

            double E = 0;
            double H = 0;

            for (int i = 0; i < length; i++) {
                ivec p = isurf.a + i*isurf.tangent;
                //H = ((*Hfield)(p)+ (*Hfield)(p - isurf.normal)
                        //+ (*Hfield)(p + isurf.tangent) + (*Hfield)(p - isurf.normal + isurf.tangent))/4;
                //E = (F->Ez(p) + F->Ez(p + isurf.tangent)
                        //+ prevE(i) + prevE(i+1))/4;

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
                        
                H = ((*Hfield)(p) + (*Hfield)(p - isurf.normal.array().cwiseAbs().matrix()))/2;
                E = (F->Ez(p) + prevE(i))/2;
                prevE(i) = F->Ez(p);
            }


            //prevE(length) = F->Ez(isurf.b);
        }

        //*** rename to flux
        Array flux() const {
            Array S = Array::Zero(freq.size());

            // for (int i = 0; i < length; i++) {
            //     for (int j = 0; j < freq.size(); j++) {
            //         S[j] += rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            //     }
            // }

            // int factor = surf.dim[0] == 0 ? -1 : +1;
            // S *= F->dx*int(surf.Sign)*factor;
            return S;
        }

        ComplexArray ntff(const vec &center, const vec &pc) const {
            vec p = pc - center;
            double r = p.norm();
            Array omega = 2*M_PI*freq;
            Array k = omega;
            ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
            ComplexArray integral = ComplexArray::Zero(freq.size());
            

            vec tangent = surf.tangent;
            vec normal = surf.normal.array().cwiseAbs();  //*** this needs to be outward normal

            double Jsgn = tangent.dot(vec(1,1));
            double Msgn = tangent.dot(vec(-1,1));

            // for (int i = 0; i < length; i++) {
            //     vec p_prime = surf.a + tangent*i*F->dx - center; //*** account for yee grid half-step here
            //     double angle = p.dot(normal)/r;  //*** worry about the sign of normal here
            //     for (int j = 0; j < freq.size(); j++) {
            //         auto E = rE(i,j) + 1i*iE(i,j);
            //         auto H = rH(i,j) + 1i*iH(i,j);

            //         auto Jeq_term = Jsgn*omega[j]*H;
            //         auto Meq_term = Msgn*k[j]*E*angle;
            //         auto integand = (Jeq_term + Meq_term)*exp(1i*k[j]*p.dot(p_prime)/r);
            //         integral[j] += integand; 

            //     }
            // }

            return factor*integral;
        }

        //equivalent currents
        //ComplexTensor Jeq() const;
        //ComplexTensor Meq() const;

    private:
        surface surf;
        Array prevE;     ///< previous electric field values
        Array freq;
        int length;      ///< length of monitor in grid points
    };

}

#endif
