#include "timer.h"
#include <iomanip>
#include "termcolor.h"
#include <algorithm>

using namespace std;

namespace qbox {

    timers::timers() {
        time_map[clock_name::looping] = timer();
        time_map[clock_name::fourier] = timer();
        time_map[clock_name::hdf5] = timer();
        time_map[clock_name::self] = timer();
        start(clock_name::self);
    }
    
    void timers::start(clock_name name) {
        time_map[name].begin();
    };

    void timers::stop(clock_name name) {
        time_map[name].end();
    }


    void timers::display() const{

        cout << termcolor::underline << 
             termcolor::bold << "\nTime Usage" << endl;
        cout << termcolor::reset;

        double other_time = duration(clock_name::self) - duration(clock_name::hdf5) - duration(clock_name::fourier) - duration(clock_name::looping);

        display_timer(clock_name::looping);
        display_timer(clock_name::fourier);
        display_timer(clock_name::hdf5);
        display_timer_string("Other", other_time);
        display_timer(clock_name::self);
    }

    timers::~timers() {
        stop(clock_name::self);
        display();
    }

    string timers::clock_string_name(clock_name name) const{
        switch(name) {
            case clock_name::fourier: return "Fourier Transforming";
            case clock_name::hdf5:    return "HDF5 Output";
            case clock_name::looping: return "Time Stepping";
            case clock_name::self: return "Total";
            default: return "Other";
        }
    }

    void timers::display_timer(clock_name c_name) const {
        string name = clock_string_name(c_name);
        if (c_name == clock_name::self)
           cout << termcolor::bold;

        display_timer_string(name, duration(c_name));
    }

    void timers::display_timer_string(string name, double duration) const {
        const int name_max_width = 25;
        const char sep = '.';

        cout << "     ";
        cout << left << setw(name_max_width) << 
                setfill(sep) << name;
        cout << left << setw(name_max_width) << 
                setfill(sep) << setprecision(5) << 
                setw(1) << duration;
        cout << " s" << endl;
        cout << termcolor::reset;
    }

    double timers::duration(clock_name c_name) const {
        auto t = time_map.find(c_name);
        return t->second.duration();
    }
}
