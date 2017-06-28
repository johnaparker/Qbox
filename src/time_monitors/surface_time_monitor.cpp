#include "field2.h"
#include "time_monitors/surface_time_monitor.h"

namespace qbox {

    surface_time_monitor::surface_time_monitor(std::string name, const surface &surf):
        time_monitor(name), surf(surf) {};
    surface_time_monitor::surface_time_monitor(const surface &surf): 
        surface_time_monitor("monitor_" + std::to_string(_num_created), surf) {_num_created++;}

    void surface_time_monitor::set_F(Field2D *newF) {
        time_monitor::set_F(newF);
        surf.write(get_group());
    }

    void surface_time_monitor::write(const fields& A) {
        switch(A) {
            case fields::E   : write(fields::Ez); return; break;
            case fields::H   : write(fields::Hx); 
                               write(fields::Hy); return; break;
            case fields::all : write(fields::E); 
                               write(fields::H);  return; break;
            default : break;
        }

        isurface isurf = F->grid.to_grid(surf);
        int length = isurf.dim.norm();
        tensor1 sub_field(length);

        tensor& field = F->get_field_ref(A);
        for (int i = 0; i < length; i++) {
            ivec pi = isurf.a + i*isurf.tangent;
            sub_field(i) = field(pi);
        }


        auto iter = dsets.find(A);
        if (iter != dsets.end())
            iter->second.append(sub_field.data());
        else {
            std::string name = field_names.at(A);
            auto h5group = get_group();
            auto dset = h5cpp::write_tensor(sub_field, h5group, name, h5cpp::append::True);
            dsets[A] = std::move(dset);
        }
    }
}