#ifndef GUARD_rank_monitor_h
#define GUARD_rank_monitor_h

#include "monitor.h"
#include "dft.h"

namespace qbox {

    template <int RANK>
    class rank_monitor: public monitor {

    public:
        rank_monitor(std::string name, std::string sub_name): monitor(name, sub_name) {};
        rank_monitor(std::string sub_name): monitor(sub_name) {};

        // virtual void update() = 0;                //update the DFT values
        void operator-=(const rank_monitor& other) {

        }

    private:
        dft<RANK> fourier;
    };

}

#endif
