#ifndef GUARD_volume_monitor_h
#define GUARD_volume_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"

namespace qbox {

    template <class T = DFT::all>
    class volume_monitor: public rank_monitor<2> {
        static constexpr char* sub_group = "volume_monitor";
    public:
        volume_monitor(std::string name, const volume &vol, const Array &freq): rank_monitor(name, sub_group, freq), vol(vol) {
            static_assert(!std::is_same<DFT::tangent,T>::value, "DFT::tangent is ambiguous for a volume monitor");
        };

        volume_monitor(const volume &vol, const Array &freq): volume_monitor("monitor_" + std::to_string(_num_created), vol, freq) {_num_created++;}


        void set_F(Field2D *newF) {
            monitor::set_F(newF);
            
            auto ivol = newF->grid.to_grid(vol);
            int Nx = ivol.dim(0) + 1;
            int Ny = ivol.dim(1) + 1;

            if constexpr (std::is_same<T, DFT::Ez>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Ez", {Nx,Ny});
            if constexpr (std::is_same<T, DFT::Hx>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Hx", {Nx,Ny});
            if constexpr (std::is_same<T, DFT::Hy>::value || std::is_same<T, DFT::all>::value)
                fourier.add("Hy", {Nx,Ny});

            auto group = get_group();
            vol.write(group);
            fourier.write_properties(group);
        }

        std::function<double(int,int)> locate(std::string comp) {
            auto ivol = F->grid.to_grid(vol);

            if (comp == "Ez") {
                return [this, &ivol](int i, int j) {
                    ivec p = ivol.a + ivec(i,j);
                    return F->Ez(p);
                };
            }

            else if (comp == "Hx") {
                return [this, &ivol](int i, int j) {
                    ivec p = ivol.a + ivec(i,j);
                    return F->Hx(p);
                };
            }

            else if (comp == "Hy") {
                return [this, &ivol](int i, int j) {
                    ivec p = ivol.a + ivec(i,j);
                    return F->Hy(p);
                };
            }
        }

        void update() {
            fourier.update<double(int,int)> (std::bind(&volume_monitor::locate, this, std::placeholders::_1), F->t);
        }

    private:
        volume vol;
    };

}

#endif
