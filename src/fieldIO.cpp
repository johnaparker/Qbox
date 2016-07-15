#include "fieldIO.h"
#include "termcolor.h"
#include <iostream>

using namespace std;

namespace qbox {

fieldIO::fieldIO(string filename, Field2D* fp): filename(filename), fp(fp) {
    if (!filename.empty()) {
        outFile = make_unique<h5cpp::h5file>(filename, h5cpp::io::w);
        cout << "\n" << termcolor::bold << termcolor::green 
             << "New simulation, creating HDF5 output file \'" << filename << "\'" 
             << termcolor::reset << endl << endl;
    }
}

void fieldIO::write_field(const fields field) {

    switch(field) {
        case fields::Ez: if (!outFile->object_exists("Fields/Ez")) {
                                 create_fields_dataset(field);
                         }
                         else {
                             auto gFields = outFile->open_group("Fields");
                             auto dset = gFields->open_dataset("Ez");
                             dset->append(fp->Ez.data());
                         }
                         break;
        case fields::Hx: if (!outFile->object_exists("Fields/Hx")) {
                                 create_fields_dataset(field);
                         }
                         else {
                             auto gFields = outFile->open_group("Fields");
                             auto dset = gFields->open_dataset("Hx");
                             dset->append(fp->Hx.data());
                         }
                         break;
        case fields::Hy: if (!outFile->object_exists("Fields/Hy")) {
                                 create_fields_dataset(field);
                         }
                         else {
                             auto gFields = outFile->open_group("Fields");
                             auto dset = gFields->open_dataset("Hy");
                             dset->append(fp->Hy.data());
                         }
                         break;
    }
}

void fieldIO::write_monitor(std::string name, double *data, int N, bool extendable)  {
    unique_ptr<h5cpp::h5group> gMonitors, gName;
    if (!outFile->object_exists("Monitors"))
        gMonitors = outFile->create_group("Monitors");
    else
        gMonitors = outFile->open_group("Monitors");

    if (!gMonitors->object_exists(name))
        gName = gMonitors->create_group(name);
    else
        gName = gMonitors->open_group(name);

    unique_ptr<h5cpp::h5dset> dset;
    if (!gName->object_exists("flux")) {
        vector<hsize_t> dims = {hsize_t(N)};
        vector<hsize_t> max_dims = {hsize_t(N)};
        if (!extendable) {
            h5cpp::dataspace ds(dims, max_dims);
            dset = gName->create_dataset("flux", 
                         h5cpp::dtype::Double, ds); 
        }
        else {
            dims.push_back(1);
            max_dims.push_back(h5cpp::inf);
            vector<hsize_t> chunk_dims = dims;
            h5cpp::dataspace ds(dims, max_dims, chunk_dims);
            dset = gName->create_dataset("flux", 
                         h5cpp::dtype::Double, ds); 
        }
        dset->write(data);
        h5cpp::dataspace ds_a(vector<hsize_t>{1});

        //*** Need to pass in freq data
        double fmin = 0;
        double fmax = 1;
        double df = .1;
        auto attr = dset->create_attribute("fmin", h5cpp::dtype::Double, ds_a);
        attr->write(&fmin);
        attr = dset->create_attribute("fmax", h5cpp::dtype::Double, ds_a);
        attr->write(&fmax);
        attr = dset->create_attribute("df", h5cpp::dtype::Double, ds_a);
        attr->write(&df);
    }
    else {
        dset = gName->open_dataset("flux");
        dset->append(data);
    }
}  

void fieldIO::create_fields_dataset(fields field) {
    unique_ptr<h5cpp::h5group> gFields;
    unique_ptr<h5cpp::h5dset> dset;

    if (!outFile->object_exists("Fields")) {
        gFields = outFile->create_group("Fields");
        h5cpp::dataspace ds_a(vector<hsize_t>{1});
        auto attr = gFields->create_attribute("dx", h5cpp::dtype::Double, ds_a);
        attr->write(&(fp->dx));
        attr = gFields->create_attribute("dt", h5cpp::dtype::Double, ds_a);
        attr->write(&(fp->dt));
    }
    else
        gFields = outFile->open_group("Fields");

    vector<hsize_t> dims = {hsize_t(fp->Nx),hsize_t(fp->Ny),1};
    vector<hsize_t> max_dims = {hsize_t(fp->Nx),hsize_t(fp->Ny),h5cpp::inf};
    vector<hsize_t> chunk_dims = dims;
    h5cpp::dataspace ds(dims, max_dims, chunk_dims, false);
    switch(field) {
        case fields::Ez:  dset = gFields->create_dataset("Ez", 
                                 h5cpp::dtype::Double, ds); 
                          dset->write(fp->Ez.data());
                          break;
        case fields::Hx:  dset = gFields->create_dataset("Hx", 
                                 h5cpp::dtype::Double, ds); 
                          dset->write(fp->Ez.data());
                          break;
        case fields::Hy:  dset = gFields->create_dataset("Hy", 
                                 h5cpp::dtype::Double, ds); 
                          dset->write(fp->Hy.data());
                          break;
    }
}

}

