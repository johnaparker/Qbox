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
    }

    h5cpp::h5group monitor::get_group() {

        auto gMonitors = outFile->create_or_open_group("Monitors");
        h5cpp::h5group gName;

        if (!gMonitors.object_exists(name)) {
            gName = gMonitors.create_group(name);

            int Nfreq = freq.size();
            auto freq_data = freq.get_freq();
            h5cpp::dspace ds_freq(vector<hsize_t>{hsize_t(Nfreq)});
            auto dset = gName.create_dataset("freq", h5cpp::dtype::Double, ds_freq);
            dset.write(freq_data.data());
        }
        else
            gName = gMonitors.open_group(name);

        return gName;
    }

    void monitor::write_flux() {
        auto S = compute_flux();
        auto gName = get_group();

        h5cpp::h5dset dset;
        if (!gName.object_exists("flux")) {
            vector<hsize_t> dims = {hsize_t(freq.size())};
            vector<hsize_t> max_dims = {hsize_t(freq.size())};
            if (!extendable) {
                h5cpp::dspace ds(dims, max_dims);
                dset = gName.create_dataset("flux", 
                             h5cpp::dtype::Double, ds); 
            }
            else {
                dims.push_back(1);
                max_dims.push_back(h5cpp::inf);
                vector<hsize_t> chunk_dims = dims;
                h5cpp::dspace ds(dims, max_dims, chunk_dims);
                dset = gName.create_dataset("flux", 
                             h5cpp::dtype::Double, ds); 
            }
            dset.write(S.data());
        }
        else {
            dset = gName.open_dataset("flux");
            dset.append(S.data());
        }
    }
}
