#include "timer.h"
#include <iomanip>
#include "termcolor.h"

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
        const int name_max_width = 25;
        const char sep = '.';
        cout << termcolor::underline << 
             termcolor::bold << "\nTime Usage" << endl;
        cout << termcolor::reset;
        for (const auto& t: time_map) {
           double duration = t.second.duration();
           string name = clock_string_name(t.first);
           if (t.first == clock_name::self)
               cout << termcolor::bold;
           cout << "     ";
           cout << left << setw(name_max_width) << 
                   setfill(sep) << name;
           cout << left << setw(name_max_width) << 
                   setfill(sep) << setprecision(5) << 
                   setw(1) << duration;
           cout << " s" << endl;
           cout << termcolor::reset;
        }
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
}
