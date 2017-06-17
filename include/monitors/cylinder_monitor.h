#ifndef GUARD_cylinder_monitor_h
#define GUARD_cylinder_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"
#include "monitors/flux.h"

namespace qbox {

    template <class T = DFT::all>
    class cylinder_monitor: public rank_monitor<1> {
        static constexpr char* sub_group = "cylinder_monitor";
    public:
        cylinder_monitor() = default;
        cylinder_monitor(std::string name, const cylinder_surface &surf, const Array &freq): rank_monitor(name, sub_group, freq), surf(surf), length(0) {
            static_assert(!std::is_same<DFT::Ez,T>::value || !std::is_same<DFT::Hx,T>::value || !std::is_same<DFT::Hy,T>::value, "Cartesian components cannot be directly DFT'd for cylinder monitors");
        }
        cylinder_monitor(const surface &cylinder_surf, const Array &freq): rank_monitor(sub_group, freq), surf(surf), length(0) {
            static_assert(!std::is_same<DFT::Ez,T>::value || !std::is_same<DFT::Hx,T>::value || !std::is_same<DFT::Hy,T>::value, "Cartesian components cannot be directly DFT'd for cylinder monitors");
        }

        void set_F(Field2D *newF) {
            monitor::set_F(newF);
            length = ceil(2*M_PI*surf.radius/F->dx);
            prevE = Array::Zero(length);

            if constexpr (std::is_same<T, DFT::all>::value) {
                fourier.add("Ez", {length});
                fourier.add("Hp", {length});
                fourier.add("Hr", {length});
            }
            if constexpr (std::is_same<T, DFT::tangent>::value) {
                fourier.add("Ez", {length});
                fourier.add("Hp", {length});
            }

            auto group = get_group();
            surf.write(group);
            fourier.write_properties(group);
        }

        std::function<double(int)> locate(std::string comp) {
            if (comp == "Ez") {
                return [this](int i) {
                    double theta = 2*M_PI*(i + 0.5)/(length + 1);
                    vec pr = surf.position(theta);
                    double tempE = F->interpolate(fields::Ez, pr);
                    double E = (tempE + prevE[i])/2;
                    prevE(i) = tempE;
                    return E;
                };
            }

            else if (comp == "Hp") {
                return [this](int i) {
                    double theta = 2*M_PI*(i + 0.5)/(length + 1);
                    vec pr = surf.position(theta);
                    return vec(F->interpolate(fields::Hx, pr), F->interpolate(fields::Hy, pr)).dot(surf.tangent(theta));
                };
            }

            else if (comp == "Hr") {
                return [this](int i) {
                    double theta = 2*M_PI*(i + 0.5)/(length + 1);
                    vec pr = surf.position(theta);
                    return vec(F->interpolate(fields::Hx, pr), F->interpolate(fields::Hy, pr)).dot(surf.normal(theta));
                };
            }
        }

        void update() {
            fourier.update<double(int)> (std::bind(&cylinder_monitor::locate, this, std::placeholders::_1), F->t);
        }

        Flux flux() const {
            static_assert(std::is_same<DFT::all,T>::value || std::is_same<DFT::tangent,T>::value, "DFT does not contain tangent");

            Array S = Array::Zero(fourier.Nfreq());
            const auto E = fourier("Ez");
            const auto H = fourier("Hp");

            for (int i = 0; i < length; i++) {
                for (int j = 0; j < fourier.Nfreq(); j++) {
                    S[j] += E.real(i,j)*H.real(i,j) + E.imag(i,j)*H.imag(i,j);
                }
            }

            S *= 2*M_PI*surf.radius/length;
            return Flux(S, *outFile, get_group());
        }

        ComplexArray ntff(const vec &center, const vec &pc) const {
            static_assert(std::is_same<DFT::all,T>::value || std::is_same<DFT::tangent,T>::value, "DFT does not contain tangent");

            vec p = pc - center;
            double r = p.norm();
            Array omega = 2*M_PI*fourier.get_freq();
            Array k = omega;
            ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
            //ComplexArray integral = ComplexArray::Zero(fourier.Nfreq());
            

            //vec tangent = surf.tangent;
            //vec normal = surf.normal.array().cwiseAbs();  /[>** this needs to be outward normal

            //double Jsgn = tangent.dot(vec(1,1));
            //double Msgn = tangent.dot(vec(-1,1));

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

            return factor;
        }

        //equivalent currents
        //ComplexTensor Jeq() const;
        //ComplexTensor Meq() const;

    private:
        cylinder_surface surf;
        Array prevE;     ///< previous electric field values
        int length;      ///< length of monitor in grid points
    };

}

#endif
