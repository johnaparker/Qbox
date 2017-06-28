#ifndef GUARD_volume_time_monitor_h
#define GUARD_volume_time_monitor_h

#include "time_monitor.h"
#include "../vec.h"

namespace qbox {

    class volume_time_monitor : public time_monitor {

    public:
        volume_time_monitor(std::string name, const volume &vol);
        volume_time_monitor(const volume &vol);

        void set_F(Field2D *newF) override;

    protected:
        void write_impl(const fields& A) override;

    private:
        volume vol;
    };

}

#endif
