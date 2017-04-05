#include "sources/source.h"
#include "field2.h"
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

//Allow amplitude specfications; allow gradual turn on for sine
//For Gaussian, get rid of T0, in favor of starting it at specified time

namespace qbox {

    int source::_num_created = 0;

    source::source(string name, string sub_group, const time_profile &_tp): name(name), sub_group(sub_group), tp(_tp.clone()) {
        F = nullptr;
        t = nullptr;
        outFile = nullptr;
    }

    source::source(string sub_group, const time_profile &_tp): 
           source("source_" + to_string(_num_created), sub_group, _tp) {_num_created++;}

    h5cpp::h5group source::get_group() {
        auto gSources = outFile->create_or_open_group("sources");
        auto gSubGroup = gSources.create_or_open_group(sub_group);
        auto my_group = gSubGroup.create_or_open_group(name);

        return my_group;
    }

    void source::set_F(Field2D *field) {
        F = field;
        t = &(F->t);
        outFile = make_unique<h5cpp::h5file>(*field->outFile.get());

        write();
    }

    void source::write() {
        auto my_group = get_group();
        tp->write(my_group);        
    }
}
