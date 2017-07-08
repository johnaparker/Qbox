#ifndef GUARD_surface_time_monitor_h
#define GUARD_surface_time_monitor_h

#include "time_monitor.h"
#include "../vec.h"

namespace qbox {

    class surface_time_monitor : public time_monitor {

    public:
        surface_time_monitor(std::string name, const surface &surf, const fields& Fields, const every& out_freq = every(0));
        surface_time_monitor(const surface &surf, const fields& Fields, const every& out_freq = every(0));

        void set_F(Field2D *newF) override;

    protected:
        void write_impl(const fields& A) override;

    private:
        surface surf;
    };

}

#endif
