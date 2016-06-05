#ifndef GUARD_box_monitor_h
#define GUARD_box_monitor_h

#include <vector>
#include <string>
#include <memory>
#include "matrix.h"
#include "monitor.h"
#include "surface_monitor.h"

namespace apine {

    class Field2D;

    //*** There is memory redunancy here....is it worth it for using a templated add_monitor function instead???
    //box monitor: monitors all points inside 4 surface monitors
    class box_monitor: public monitor {
    public:
        //various constructors
        box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, std::shared_ptr<freq_data> freq, int N);
        box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N);
        box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f);

        box_monitor() = default;
        box_monitor(const box_monitor&) = default;
        box_monitor(box_monitor&&) = default;
        box_monitor& operator=(box_monitor&&) = default;
        
        //same as surface_monitor 
        void set_freq(std::shared_ptr<freq_data> new_freq);
        void set_F(Field2D *newF);
        void update();
        void compute_flux(double *S);
        void write(std::string filename, bool extendable = false);

        void write_sides(std::string filename, bool extendable = false);  //call write for all surface_monitors

    public:
        surface_monitor monitors[4];    //4 surface monitors
    };
}

#endif
