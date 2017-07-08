#ifndef GUARD_point_time_monitor_h
#define GUARD_point_time_monitor_h

#include "time_monitor.h"
#include "../vec.h"

namespace qbox {

    class point_time_monitor : public time_monitor {

    public:
        point_time_monitor(std::string name, const vec &pos, const fields& Fields, const every& out_freq = every(0));
        point_time_monitor(const vec &pos, const fields& Fields, const every& out_freq = every(0));

        void set_F(Field2D *newF) override;

    protected:
        void write_impl(const fields& A) override;

    private:
        vec pos;
    };

}

#endif
