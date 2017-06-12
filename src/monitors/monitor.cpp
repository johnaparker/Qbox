#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "field2.h"
#include "monitors/monitor.h"
#include "monitors/freq.h"

using namespace std;



namespace qbox {

    int monitor::_num_created = 0;

    void monitor::set_F(Field2D *newF) {
        F = newF;
        outFile = make_unique<h5cpp::h5file>(*newF->outFile.get());

        auto my_group = get_group();
        freq.write(my_group);
    }

    h5cpp::h5group monitor::get_group() const {

        auto gMonitors = outFile->create_or_open_group("monitors");
        auto gSubMonitors = gMonitors.create_or_open_group(sub_name);
        auto my_group = gSubMonitors.create_or_open_group(name);

        return my_group;
    }

    void monitor::write_flux() const {
        auto S = compute_flux();
        auto my_group = get_group();

        if (!my_group.object_exists("flux")) {
            h5cpp::write_array<double>(S, my_group, "flux", h5cpp::append{extendable});
        }
        else {
            auto dset = my_group.open_dataset("flux");
            dset.append(S.data());
        }
    }
}
