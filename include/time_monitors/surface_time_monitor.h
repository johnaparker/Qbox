#ifndef GUARD_surface_time_monitor_h
#define GUARD_surface_time_monitor_h

#include "time_monitor.h"
#include "../vec.h"

namespace qbox {

    class surface_time_monitor : public time_monitor {

    public:
        surface_time_monitor(std::string name, const surface &surf);
        surface_time_monitor(const surface &surf);

        void write(const fields& A) const override;

    private:
        surface surf;
    };

}

#endif
