#ifndef GUARD_surface_monitor_h
#define GUARD_surface_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"

namespace qbox {

    //*** Perhaps allow a res parameter to "skip" some grid cells 
    //surface monitor: monitors all points inside a plane
    class Flux {
    public:
        Flux(const Array& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
            file_name = file.get_name();
            group_path = group.get_path();
        }

        void write() {
            h5cpp::h5file f(file_name, h5cpp::io::rw);
            auto g = f.open_group(group_path);
            h5cpp::write_array<double>(S, g, "flux");
        }

        Array flux() {return S;}

    private:
        Array S;
        std::string file_name;
        std::string group_path;
    };

    template <class T = DFT::tangent>
    class surface_monitor: public rank_monitor<1> {
        static constexpr char* sub_group = "surface_monitor";
    public:
        surface_monitor(std::string name, const surface &surf, const Array &freq): rank_monitor(name, sub_group, freq), surf(surf), length(0) {};
        surface_monitor(const surface &surf, const Array &freq): rank_monitor(sub_group, freq), surf(surf), length(0) {};

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
                return [this, &isurf](int i) {
                    ivec p = isurf.a + i*isurf.tangent;
                    double E = F->Ez(p);
                    prevE(i) = F->Ez(p);
                    return E;
                };
            }

            else if (comp == "Hx") {
                auto *Hfield = &F->Hx;
                return [this, &isurf, Hfield](int i) {
                    ivec p = isurf.a + i*isurf.tangent;
                    return ((*Hfield)(p) + (*Hfield)(p - isurf.normal.array().cwiseAbs().matrix()))/2;
                };
            }

            else if (comp == "Hy") {
                auto *Hfield = &F->Hy;
                return [this, &isurf, Hfield](int i) {
                    ivec p = isurf.a + i*isurf.tangent;
                    return ((*Hfield)(p) + (*Hfield)(p - isurf.normal.array().cwiseAbs().matrix()))/2;
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

            Array S = Array::Zero(fourier.Nfreq());
            const auto E = fourier("Ez");
            const auto H = surf.dim[0] == 0 ? fourier("Hy") : fourier("Hx");

            for (int i = 0; i < length; i++) {
                for (int j = 0; j < fourier.Nfreq(); j++) {
                    S[j] += E.real(i,j)*H.real(i,j) + E.imag(i,j)*H.imag(i,j);
                }
            }

            int factor = surf.dim[0] == 0 ? -1 : +1;
            S *= F->dx*int(surf.Sign)*factor;
            return Flux(S, *outFile, get_group());
        }

        ComplexArray ntff(const vec &center, const vec &pc) const {
            static_assert(std::is_same<DFT::all,T>::value || std::is_same<DFT::tangent,T>::value, "DFT does not contain tangent");

            vec p = pc - center;
            double r = p.norm();
            Array omega = 2*M_PI*fourier.get_freq();
            Array k = omega;
            ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
            ComplexArray integral = ComplexArray::Zero(fourier.Nfreq());
            

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
        int length;      ///< length of monitor in grid points
    };

}

#endif
