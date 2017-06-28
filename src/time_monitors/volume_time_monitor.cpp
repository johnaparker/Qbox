#include "field2.h"
#include "time_monitors/volume_time_monitor.h"

namespace qbox {

    volume_time_monitor::volume_time_monitor(std::string name, const volume &vol):
        time_monitor(name), vol(vol) {};
    volume_time_monitor::volume_time_monitor(const volume &vol): 
        volume_time_monitor("monitor_" + std::to_string(_num_created), vol) {_num_created++;}

    void volume_time_monitor::set_F(Field2D *newF) {
        time_monitor::set_F(newF);
        vol.write(get_group());
    }

    void volume_time_monitor::write_impl(const fields& A) {
        const ivolume ivol = F->grid.to_grid(vol);
        const int Nx = ivol.dim(0);
        const int Ny = ivol.dim(1);
        tensor sub_field(Nx,Ny);

        tensor& field = F->get_field_ref(A);
        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++) {
                ivec pi = ivol.a + ivec(i,j);
                sub_field(i,j) = field(pi);
            }
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