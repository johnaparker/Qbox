#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/monitor.h"
#include "monitors/freq.h"

using namespace std;



namespace qbox {

    void monitor::set_F(Field2D *newF) {
        F = newF;
        outFile = make_unique<h5cpp::h5file>(*newF->outFile.get());

        auto my_group = get_group();
        freq.write(my_group);
    }

    h5cpp::h5group monitor::get_group() {

        auto gMonitors = outFile->create_or_open_group("monitors");
        auto gSubMonitors = gMonitors.create_or_open_group(sub_name);
        auto my_group = gSubMonitors.create_or_open_group(name);

        return my_group;
    }

    void monitor::write_flux() {
        auto S = compute_flux();
        auto my_group = get_group();

        h5cpp::h5dset dset;
        if (!my_group.object_exists("flux")) {
            vector<hsize_t> dims = {hsize_t(freq.size())};
            vector<hsize_t> max_dims = {hsize_t(freq.size())};
            if (!extendable) {
                h5cpp::dspace ds(dims, max_dims);
                dset = my_group.create_dataset("flux", 
                             h5cpp::dtype::Double, ds); 
            }
            else {
                dims.push_back(1);
                max_dims.push_back(h5cpp::inf);
                vector<hsize_t> chunk_dims = dims;
                h5cpp::dspace ds(dims, max_dims, chunk_dims);
                dset = my_group.create_dataset("flux", 
                             h5cpp::dtype::Double, ds); 
            }
            dset.write(S.data());
        }
        else {
            dset = my_group.open_dataset("flux");
            dset.append(S.data());
        }
    }
}
