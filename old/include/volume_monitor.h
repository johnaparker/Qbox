#ifndef GUARD_volume_monitor_h
#define GUARD_volume_monitor_h

#include <string>
#include <memory>
#include "../vec.h"
#include "monitor.h"


namespace qbox {

    class Field2D; 

    class volume_monitor: public monitor {

    public:
        volume_monitor() = default;
        volume_monitor(std::string name, const volume &vol, const freq_data &freq);   //using freq data
        volume_monitor(const volume &vol, const freq_data &freq);

        Array compute_flux() const override {};
        void set_F(Field2D *newF);    //set ownership
        void update();   //update the DFT matrices

        void write_dft() const;

    private:
        volume vol;
        tensor3 rE, iE;    //DFT matrices
        int Nx, Ny, Nfreq;
    };

}

#endif
