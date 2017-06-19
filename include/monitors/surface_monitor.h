#ifndef GUARD_surface_monitor_h
#define GUARD_surface_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"

#include "flux.h"
#include "ntff.h"
#include "force.h"

namespace qbox {

    //*** Perhaps allow a res parameter to "skip" some grid cells 
    //surface monitor: monitors all points inside a plane
    template <class T = DFT::tangent>
    class surface_monitor: public rank_monitor<1> {
        static constexpr char const* sub_group = "surface_monitor";
    public:
        surface_monitor() = default;
        surface_monitor(std::string name, const surface &surf, const Array &freq): rank_monitor(name, sub_group, freq), surf(surf), length(0) {};
        surface_monitor(const surface &surf, const Array &freq): surface_monitor("monitor_" + std::to_string(_num_created), surf, freq) {_num_created++;}


        void set_F(Field2D *newF) {
            monitor::set_F(newF);
            auto isurf = newF->grid.to_grid(surf);
            length = isurf.dim.norm();

            prevE = Array::Zero(length);

            if constexpr (std::is_same<T, DFT::Ez>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Ez", {length});
            if constexpr (std::is_same<T, DFT::Hx>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Hx", {length});
            if constexpr (std::is_same<T, DFT::Hy>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Hy", {length});
            if constexpr (std::is_same<T, DFT::tangent>::value) {
                fourier.add("Ez", {length});
                if (isurf.dim[0] == 0)
                    fourier.add("Hy", {length});
                else
                    fourier.add("Hx", {length});
            }

            // if (isurf.dim[0] == 0)
            //     fourier.add("Hy", {length});
            // else
            //     fourier.add("Hx", {length});

            auto group = get_group();
            surf.write(group);
            fourier.write_properties(group);
        }

        std::function<double(int)> locate(std::string comp) {
            auto isurf = F->grid.to_grid(surf);

            if (comp == "Ez") {
                auto *Efield = &F->Ez;
                return [this, &isurf, Efield](int i) {
                    ivec p = isurf.a + i*isurf.tangent;
                    double tempE = (*Efield)(p);
                    double E = (tempE + prevE(i))/2;
                    prevE(i) = tempE;
                    return E;
                };
            }

            else if (comp == "Hx") {
                auto *Hfield = &F->Hx;
                return [this, &isurf, Hfield](int i) {
                    ivec p = isurf.a + i*isurf.tangent;
                    return ((*Hfield)(p) + (*Hfield)(p - ivec(0,1)))/2;
                };
            }

            else if (comp == "Hy") {
                auto *Hfield = &F->Hy;
                return [this, &isurf, Hfield](int i) {
                    ivec p = isurf.a + i*isurf.tangent;
                    return ((*Hfield)(p) + (*Hfield)(p - ivec(1,0)))/2;
                };
            }
        }

        void update() {
            fourier.update<double(int)> (std::bind(&surface_monitor::locate, this, std::placeholders::_1), F->t);

            // auto isurf = F->grid.to_grid(surf);
            // fourier.update<double(int)> ([this,&isurf](std::string comp)->std::function<double(int)> {
            //     if (comp == "Ez") {
            //         return [this, &isurf](int i) {
            //             ivec p = isurf.a + i*isurf.tangent;
            //             double E = F->Ez(p);
            //             prevE(i) = F->Ez(p);
            //             return E;
            //         };
            //     }

            //     else if (comp == "Hx") {
            //         auto *Hfield = &F->Hx;
            //         return [this, &isurf, Hfield](int i) {
            //             ivec p = isurf.a + i*isurf.tangent;
            //             return ((*Hfield)(p) + (*Hfield)(p - isurf.normal.array().cwiseAbs().matrix()))/2;
            //         };
            //     }

            //     else if (comp == "Hy") {
            //         auto *Hfield = &F->Hy;
            //         return [this, &isurf, Hfield](int i) {
            //             ivec p = isurf.a + i*isurf.tangent;
            //             return ((*Hfield)(p) + (*Hfield)(p - isurf.normal.array().cwiseAbs().matrix()))/2;
            //         };
            //     }
            // }, F->t);

            //*** this dir, Hfield combo is bad design (maybe use enum)
            // auto isurf = F->grid.to_grid(surf);
            // auto *Hfield = isurf.dim[0] == 0 ? &F->Hy : &F->Hx;

            // ComplexTensor1 Ez(length);
            // ComplexTensor1 H(length);

            // for (int i = 0; i < length; i++) {
            //     ivec p = isurf.a + i*isurf.tangent;
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
                        
                // H(i) = ((*Hfield)(p) + (*Hfield)(p - isurf.normal.array().cwiseAbs().matrix()))/2;
                // Ez(i) = (F->Ez(p) + prevE(i))/2;
                // prevE(i) = F->Ez(p);
            // }
            //prevE(length) = F->Ez(isurf.b);
            // fourier.update({{"Ez", Ez}, {"H", H}}, F->t);
        }

        Flux flux() const {
            static_assert(std::is_same<DFT::all,T>::value || std::is_same<DFT::tangent,T>::value, "DFT does not contain tangent");

            const auto E = fourier("Ez");
            const auto H = surf.dim[0] == 0 ? fourier("Hy") : fourier("Hx");

            int factor = surf.dim[0] == 0 ? -1 : +1;
            double da = factor*F->dx;
            Array S = compute_flux(E,H,da,surf.Sign);

            return Flux(S, *outFile, get_group());
        }

        ntff_point ntff(const vec &center, const vec &pc) const {
            static_assert(std::is_same<DFT::all,T>::value || std::is_same<DFT::tangent,T>::value, "DFT does not contain tangent");

            const int Nfreq = fourier.Nfreq();

            vec p = pc - center;
            double r = p.norm();
            Array omega = 2*M_PI*fourier.get_freq();
            Array k = omega;
            ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
            ComplexArray integral = ComplexArray::Zero(Nfreq);
            
            vec tangent = surf.tangent;
            vec normal = surf.normal.array().cwiseAbs();  //*** this needs to be outward normal

            double Jsgn = tangent.dot(vec(1,1));
            double Msgn = tangent.dot(vec(-1,1));

            const auto E = fourier("Ez");
            const auto H = surf.dim[0] == 0 ? fourier("Hy") : fourier("Hx");


             for (int i = 0; i < length; i++) {
                 vec p_prime = surf.a + tangent*i*F->dx - center; //*** account for yee grid half-step here
                 double angle = p.dot(normal)/r;  //*** worry about the sign of normal here
                 for (int j = 0; j < Nfreq; j++) {
                     auto Ec = E.real(i,j) + 1i*E.imag(i,j);
                     auto Hc = H.real(i,j) + 1i*H.imag(i,j);

                     auto Jeq_term = Jsgn*omega[j]*Hc;
                     auto Meq_term = Msgn*k[j]*Ec*angle;
                     auto integand = (Jeq_term + Meq_term)*exp(1i*k[j]*p.dot(p_prime)/r);
                     integral[j] += integand; 
                 }
             }

             return ntff_point(factor*integral, *outFile, get_group());
        }

        Force partial_force() const {
            static_assert(std::is_same<DFT::all,T>::value, "DFT::all required for partial force calculation");

            const int Nfreq = fourier.Nfreq();
            const auto E = fourier("Ez");
            const auto Hx = fourier("Hx");
            const auto Hy = fourier("Hy");
            const vec normal = surf.normal;

            double da = F->dx;
            tensor S(2, Nfreq); S.setZero();

            for (int i = 0; i < length; i++) {
                for (int j = 0; j < Nfreq; j++) {
                    auto Hxc = Hx.real(i,j) + 1i*Hx.imag(i,j);
                    auto Hyc = Hy.real(i,j) + 1i*Hy.imag(i,j);
                    auto Hsq = std::norm(Hxc) + std::norm(Hyc);
                    Eigen::Matrix2d sigma;
                    sigma << std::norm(Hxc) - 0.5*Hsq     , std::real(std::conj(Hxc)*Hyc),
                             std::real(Hxc*std::conj(Hyc)), std::norm(Hyc) - 0.5*Hsq;   // Maxwell Stress Tensor for 2D TM

                    vec dF = sigma*normal*da;
                    S(0,j) += dF(0);
                    S(1,j) += dF(1);
                }
            }

            return Force(S, *outFile, get_group());
        }

        Torque partial_torque(const vec &center) const {
            static_assert(std::is_same<DFT::all,T>::value, "DFT::all required for partial force calculation");

            const int Nfreq = fourier.Nfreq();
            const auto E = fourier("Ez");
            const auto Hx = fourier("Hx");
            const auto Hy = fourier("Hy");
            const vec normal = surf.normal;

            double da = F->dx;
            Array S = Array::Zero(Nfreq);

            for (int i = 0; i < length; i++) {
                vec r = surf.a + i*surf.tangent*F->dx - center;
                for (int j = 0; j < Nfreq; j++) {
                    auto Ezc = E.real(i,j) +  1i*E.imag(i,j);
                    auto Hxc = Hx.real(i,j) + 1i*Hx.imag(i,j);
                    auto Hyc = Hy.real(i,j) + 1i*Hy.imag(i,j);
                    auto Hsq = std::norm(Hxc) + std::norm(Hyc);
                    auto Esq = std::norm(Ezc) + std::norm(Ezc);
                    Eigen::Matrix2d sigma;
                    sigma << std::norm(Hxc) - 0.5*Hsq -0.5*Esq    , std::real(std::conj(Hxc)*Hyc),
                             std::real(Hxc*std::conj(Hyc)), std::norm(Hyc) - 0.5*Hsq-0.5*Esq;   // Maxwell Stress Tensor for 2D TM

                    vec dF = sigma*normal*da;
                    double tau = r(0)*dF(1) - r(1)*dF(0);
                    S(j) += tau;
                }
            }

            return Torque(S, *outFile, get_group());
        }

        //equivalent currents
        //ComplexTensor Jeq() const;
        //ComplexTensor Meq() const;

    private:
        surface surf;
        Array prevE;     ///< previous electric field values
        int length;      ///< length of monitor in grid points
    };

}

#endif
