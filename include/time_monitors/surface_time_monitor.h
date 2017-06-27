#ifndef GUARD_surface_time_monitor_h
#define GUARD_surface_time_monitor_h

#include "time_monitor.h"
#include "../vec.h"
#include <unordered_map>

namespace qbox {

    class surface_time_monitor : public time_monitor {

    public:
        surface_time_monitor(std::string name, const surface &surf);
        surface_time_monitor(const surface &surf);

        void set_F(Field2D *newF) override;
        void write(const fields& A) override;

    private:
        surface surf;
        std::unordered_map<fields, h5cpp::h5dset> dsets;
    };

}

#endif
