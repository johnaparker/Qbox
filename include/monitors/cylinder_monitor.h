#ifndef GUARD_cylinder_monitor_h
#define GUARD_cylinder_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"
#include "flux.h"
#include "ntff.h"

namespace qbox {

    template <class T = DFT::all>
    class cylinder_monitor: public rank_monitor<1> {
        static constexpr char const* sub_group = "cylinder_monitor";
    public:
        cylinder_monitor() = default;
        cylinder_monitor(std::string name, const cylinder_surface &surf, const Array &freq): rank_monitor(name, sub_group, freq), surf(surf), length(0) {
            static_assert(!std::is_same<DFT::Ez,T>::value && !std::is_same<DFT::Hx,T>::value && !std::is_same<DFT::Hy,T>::value, "Cartesian components cannot be directly DFT'd for cylinder monitors");
        }

        cylinder_monitor(const cylinder_surface &surf, const Array &freq): cylinder_monitor("monitor_" + std::to_string(_num_created), surf, freq) {_num_created++;}

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
                    double theta = 2*M_PI*(i + 0.5)/length;
                    vec pr = surf.position(theta);
                    double tempE = F->interpolate(fields::Ez, pr);
                    double E = (tempE + prevE[i])/2;
                    prevE(i) = tempE;
                    return E;
                };
            }

            else if (comp == "Hp") {
                return [this](int i) {
                    double theta = 2*M_PI*(i + 0.5)/length;
                    vec pr = surf.position(theta);
                    return vec(F->interpolate(fields::Hx, pr), F->interpolate(fields::Hy, pr)).dot(surf.tangent(theta));
                };
            }

            else if (comp == "Hr") {
                return [this](int i) {
                    double theta = 2*M_PI*(i + 0.5)/length;
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

            const auto E = fourier("Ez");
            const auto H = fourier("Hp");
            double da = 2*M_PI*surf.radius/length;
            Array S = compute_flux(E,H,da,sign::Negative);

            return Flux(S, *outFile, get_group());
        }

        ntff_point ntff(const vec &pc) const {
            static_assert(std::is_same<DFT::all,T>::value || std::is_same<DFT::tangent,T>::value, "DFT does not contain tangent");

            const int Nfreq = fourier.Nfreq();

            vec p = pc - surf.center;
            double r = p.norm();
            Array omega = 2*M_PI*fourier.get_freq();
            Array k = omega;
            ComplexArray factor = Eigen::exp(1i*(M_PI/4 - k*r))/Eigen::sqrt(8*r*M_PI*k); 
            ComplexArray integral = ComplexArray::Zero(Nfreq);
            

            const auto E = fourier("Ez");
            const auto H = fourier("Hp");

            for (int i = 0; i < length; i++) {
                double theta = 2*M_PI*(i + 0.5)/length;
                vec tangent = surf.tangent(theta);
                vec normal = surf.normal(theta);
                vec p_prime = surf.position(theta) - surf.center; //*** account for yee grid half-step here
                double angle = p.dot(normal)/r;  //*** worry about the sign of normal here
                for (int j = 0; j < Nfreq; j++) {
                    auto Ec = E.real(i,j) + 1i*E.imag(i,j);
                    auto Hc = H.real(i,j) + 1i*H.imag(i,j);

                    auto Jeq_term = 1*omega[j]*Hc;
                    auto Meq_term = -1*k[j]*Ec*angle;
                    auto integand = (Jeq_term + Meq_term)*exp(1i*k[j]*p.dot(p_prime)/r);
                    integral[j] += integand; 
                }
            }

            return ntff_point(factor*integral, *outFile, get_group());
        }
        
        Force force() const {
            static_assert(std::is_same<DFT::all,T>::value, "DFT::all required for partial force calculation");
            
            const auto Ez = fourier("Ez");
            const auto Ht = fourier("Hp");
            const auto Hn = fourier("Hr");
            double da = 2*M_PI*surf.radius/length;
            
            tensor S = compute_force(Ez, Ht, Hn, 
                            [this](int i) {
                                double theta = 2*M_PI*(i + 0.5)/length;
                                return surf.tangent(theta);
                            },
                            [this](int i) {
                                double theta = 2*M_PI*(i + 0.5)/length;
                                return surf.normal(theta);
                            }, 
                            da, sign::Positive);

            return Force(S, *outFile, get_group());
        }

        Torque torque() const {
            static_assert(std::is_same<DFT::all,T>::value, "DFT::all required for partial force calculation");
            
            const auto Ez = fourier("Ez");
            const auto Ht = fourier("Hp");
            const auto Hn = fourier("Hr");
            double da = 2*M_PI*surf.radius/length;
            
            Array S = compute_torque(Ez, Ht, Hn, 
                            [this](int i) {
                                double theta = 2*M_PI*(i + 0.5)/length;
                                return surf.position(theta) - surf.center;
                            },
                            [this](int i) {
                                double theta = 2*M_PI*(i + 0.5)/length;
                                return surf.tangent(theta);
                            },
                            [this](int i) {
                                double theta = 2*M_PI*(i + 0.5)/length;
                                return surf.normal(theta);
                            }, 
                            da, sign::Positive);

            return Torque(S, *outFile, get_group());
        }

        ntff_sphere ntff(double radius, int N) const {
            const int Nfreq = fourier.Nfreq();

            ComplexTensor result(N, Nfreq);
            Array theta = Array::LinSpaced(N, 0, 2*M_PI);

            for (int i = 0; i < N; i++) {
                vec p = radius*vec(cos(theta[i]), sin(theta[i])) + surf.center;
                ComplexArray p_ntff = ntff(p).data();
                for (int j = 0; j < Nfreq; j++)
                    result(i,j) = p_ntff(j);
            }
            return ntff_sphere(result, *outFile, get_group());
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
