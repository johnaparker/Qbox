#include "field2.h"
#include "time_monitors/time_monitor.h"

namespace qbox {

    int time_monitor::_num_created = 0;

    void time_monitor::set_F(Field2D *newF) {
        F = newF;
        outFile = std::make_unique<h5cpp::h5file>(*newF->outFile.get());
    }

    h5cpp::h5group time_monitor::get_group() const {
        auto gMonitors = outFile->create_or_open_group("fields");
        auto my_group = gMonitors.create_or_open_group(name);

        return my_group;
    }

    void time_monitor::write_if_ready() {
        if (out_freq.check_repeat())
            write();
    }

    void time_monitor::write() {
        write(Fields);
    }

    void time_monitor::write(const fields& A) {
        switch(A) {
            case fields::E:    write(fields::Ez); break;
            case fields::H:    write(fields::Hx); 
                               write(fields::Hy); break;
            case fields::all:  write(fields::E); 
                               write(fields::H);  break;
            default:           write_impl(A);     break;
        }
    }

}