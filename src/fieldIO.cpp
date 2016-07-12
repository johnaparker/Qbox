#include "fieldIO.h"

using namespace std;

namespace qbox {

fieldIO::fieldIO(string filename, Field2D* fp): filename(filename), fp(fp) {
    if (!filename.empty())
        outFile = make_unique<h5cpp::h5file>(filename, h5cpp::io::w);
}

void fieldIO::write_field(const fields field) {
    unique_ptr<h5cpp::h5group> gFields;
    if (!outFile->object_exists("Fields"))
        gFields = outFile->create_group("Fields");
    else
        gFields = outFile->open_group("Fields");

    unique_ptr<h5cpp::h5dset> dset;
    switch(field) {
        case fields::Ez: if (!outFile->object_exists("Fields/Ez")) {
                vector<hsize_t> dims = {hsize_t(fp->Nx),hsize_t(fp->Ny),1};
                vector<hsize_t> max_dims = {hsize_t(fp->Nx),hsize_t(fp->Ny),h5cpp::inf};
                vector<hsize_t> chunk_dims = dims;
                h5cpp::dataspace ds(dims, max_dims, chunk_dims, false);
                dset = gFields->create_dataset("Ez", 
                             h5cpp::dtype::Double, ds); 
                dset->write(fp->Ez.data());

                h5cpp::dataspace ds_a(vector<hsize_t>{1});
                auto attr = dset->create_attribute("dx", h5cpp::dtype::Double, ds_a);
                attr->write(&(fp->dx));
                attr = dset->create_attribute("dt", h5cpp::dtype::Double, ds_a);
                attr->write(&(fp->dt));

             }
             else {
                 dset = gFields->open_dataset("Ez");
                 dset->append(fp->Ez.data());
             }
             break;
    }
}

void fieldIO::write_monitor(std::string name, double *data, int N, bool extendable)  {
    unique_ptr<h5cpp::h5group> gFields;
    if (!outFile->object_exists("Monitors"))
        gFields = outFile->create_group("Monitors");
    else
        gFields = outFile->open_group("Monitors");

    unique_ptr<h5cpp::h5dset> dset;
    if (!outFile->object_exists("Monitors/"+name)) {
        vector<hsize_t> dims = {hsize_t(N)};
        vector<hsize_t> max_dims = {hsize_t(N)};
        //vector<hsize_t> chunk_dims = dims;
        h5cpp::dataspace ds(dims, max_dims);
        dset = gFields->create_dataset(name, 
                     h5cpp::dtype::Double, ds); 
        dset->write(data);
    }
    else {
        dset = gFields->open_dataset(name);
        dset->append(data);
    }
}  


}
