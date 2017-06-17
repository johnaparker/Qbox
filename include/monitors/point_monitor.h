#ifndef GUARD_point_monitor_h
#define GUARD_point_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"

namespace qbox {

    template <class T = DFT::all>
    class point_monitor: public rank_monitor<0> {
        static constexpr char* sub_group = "point_monitor";
    public:
        point_monitor(std::string name, const vec &pos, const Array &freq): rank_monitor(name, sub_group, freq), pos(pos) {
            static_assert(!std::is_same<DFT::tangent,T>::value, "DFT::tangent is ambiguous for a point monitor");
        };
        point_monitor(const vec &pos, const Array &freq): rank_monitor(sub_group, freq), pos(pos) {
            static_assert(!std::is_same<DFT::tangent,T>::value, "DFT::tangent is ambiguous for a point monitor");
        };

        void set_F(Field2D *newF) {
            monitor::set_F(newF);

            if constexpr (std::is_same<T, DFT::Ez>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Ez", {});
            if constexpr (std::is_same<T, DFT::Hx>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Hx", {});
            if constexpr (std::is_same<T, DFT::Hy>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Hy", {});

            auto group = get_group();
            h5cpp::write_vector<double>(pos, group, "position");
            fourier.write_properties(group);
        }

        std::function<double()> locate(std::string comp) {
            auto ipos = F->grid.to_grid(pos);

            if (comp == "Ez") {
                return [this, &ipos]() {
                    double E = (F->Ez(ipos)+prevE)/2;
                    prevE = F->Ez(ipos);
                    return E;
                };
            }

            else if (comp == "Hx") {
                auto *Hfield = &F->Hx;
                return [this, &ipos, Hfield]() {
                    return ((*Hfield)(ipos) + (*Hfield)(ipos - ivec(0,1)))/2;
                };
            }

            else if (comp == "Hy") {
                auto *Hfield = &F->Hy;
                return [this, &ipos, Hfield]() {
                    return ((*Hfield)(ipos) + (*Hfield)(ipos - ivec(1,0)))/2;
                };
            }
        }

        void update() {
            fourier.update<double()> (std::bind(&point_monitor::locate, this, std::placeholders::_1), F->t);
        }

    private:
        vec pos;
        double prevE;     ///< previous electric field values
    };

}

#endif
