#ifndef GUARD_box_monitor_h
#define GUARD_box_monitor_h

#include <string>
#include <memory>
#include "../matrix.h"
#include "monitor.h"
#include "surface_monitor.h"

namespace qbox {

    class Field2D;

    //*** There is memory redunancy here....is it worth it for using a templated add_monitor function instead???
    //box monitor: monitors all points inside 4 surface monitors
    class box_monitor: public monitor {
    public:
        //various constructors
        box_monitor(std::string name, const volume &vol, std::shared_ptr<freq_data> freq, int N, bool extendable=false);
        box_monitor(std::string name, const volume &vol, double fmin, double fmax, int N, bool extendable=false);
        box_monitor(std::string name, const volume &vol, double f, bool extendable=false);

        box_monitor() = default;
        box_monitor(const box_monitor&) = default;
        box_monitor(box_monitor&&) = default;
        box_monitor& operator=(box_monitor&&) = default;
        
        //same as surface_monitor 
        void set_freq(std::shared_ptr<freq_data> new_freq);
        void set_F(Field2D *newF);
        void update();
        std::unique_ptr<double[]> compute_flux() const;
        void write_flux();

        void write_flux_sides();  //call write for all surface_monitors

    private:
        surface_monitor monitors[4];    //4 surface monitors
        vec p1, p2;          //physical corners
    };
}

#endif
