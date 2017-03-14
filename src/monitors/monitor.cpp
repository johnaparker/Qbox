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

    void monitor::set_freq(shared_ptr<freq_data> new_freq) {
        //freq = unique_ptr<double[]> (new double(*new_freq));
        freq = new_freq;
    }

    void monitor::set_F(Field2D *newF) {
        F = newF;
        outFile = make_unique<h5cpp::h5file>(*newF->outFile.get());
    }

    h5cpp::h5group monitor::get_group() {
        h5cpp::h5group gMonitors, gName;
        if (!outFile->object_exists("Monitors"))
            gMonitors = outFile->create_group("Monitors");
        else
            gMonitors = outFile->open_group("Monitors");

        if (!gMonitors.object_exists(name)) {
            gName = gMonitors.create_group(name);

            int Nfreq = freq->size();
            auto freq_data = freq->get_freq();
            h5cpp::dspace ds_freq(vector<hsize_t>{hsize_t(Nfreq)});
            auto dset = gName.create_dataset("freq", h5cpp::dtype::Double, ds_freq);
            dset.write(freq_data.data());
        }
        else
            gName = gMonitors.open_group(name);

        return gName;
    }

    int get_direction(vector<int> p1, vector<int> p2) {
        if (p1[0] == p2[0])
            return 1;
        else if (p1[1] == p2[1])
            return 0;
        else
            return 2;
    }
}
