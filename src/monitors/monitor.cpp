#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "field2.h"
#include "monitors/monitor.h"

using namespace std;



namespace qbox {

    int monitor::_num_created = 0;

    void monitor::set_F(Field2D *newF) {
        F = newF;
        outFile = make_unique<h5cpp::h5file>(*newF->outFile.get());
    }

    h5cpp::h5group monitor::get_group() const {
        auto gMonitors = outFile->create_or_open_group("monitors");
        auto gSubMonitors = gMonitors.create_or_open_group(sub_name);
        auto my_group = gSubMonitors.create_or_open_group(name);

        return my_group;
    }
}
