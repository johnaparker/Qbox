#include "timer.h"
#include <iomanip>
#include "termcolor.h"

using namespace std;

namespace qbox {

    timers::timers() {
        time_map["Time Stepping"] = timer();
        time_map["Fourier Transforming"] = timer();
        time_map["HDF5 Output"] = timer();
    }
    
    void timers::start(string name) {
        time_map[name].begin();
    };

    void timers::stop(string name) {
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
           cout << "     ";
           cout << left << setw(name_max_width) << 
                   setfill(sep) << t.first;
           cout << left << setw(name_max_width) << 
                   setfill(sep) << setprecision(5) << 
                   setw(1) << duration;
           cout << " s" << endl;
        }
    }

    timers::~timers() {
        display();
    }
}
