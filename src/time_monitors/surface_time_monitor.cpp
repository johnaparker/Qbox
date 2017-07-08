#include "field2.h"
#include "time_monitors/surface_time_monitor.h"

namespace qbox {

    surface_time_monitor::surface_time_monitor(std::string name, const surface &surf, const fields& Fields, const every& out_freq ):
        time_monitor(name, Fields, out_freq), surf(surf) {};
    surface_time_monitor::surface_time_monitor(const surface &surf, const fields& Fields, const every& out_freq): 
        surface_time_monitor("monitor_" + std::to_string(_num_created), surf, Fields, out_freq) {_num_created++;}

    void surface_time_monitor::set_F(Field2D *newF) {
        time_monitor::set_F(newF);
        surf.write(get_group());
    }

    void surface_time_monitor::write_impl(const fields& A) {
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