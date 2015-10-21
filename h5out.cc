#include "h5out.h"
#include <vector>


using namespace std;
using namespace H5;

//For all code: consider when to pass by reference

node::node(H5std_string name, H5File & head, vector<hsize_t> size, vector<hsize_t> maxsize):name(name) {
    rank = size.size();
    fill_value = 0;
    dims = new hsize_t[rank];
    maxdims = new hsize_t[rank];
    hsize_t *chunk_dims = new hsize_t[rank];
    for (int i = 0; i != rank; i++) {
        dims[i] = size[i];
        maxdims[i] = maxsize[i];
        chunk_dims[i] = data[i];
    }

    DSetCreatPropList cparms;
    cparms.setChunk(rank, chunk_dims);
    cparms.setFillValue(PredType::NATIVE_DOUBLE, &fill_value);
    
    dataspace = mspace(rank, dims, maxdims);
    dataset = head.createDataSet(name, PredType::NATIVE_DOUBLE, mspace, cparms);

    extendable = true;  //obviously, fix this
}

void node::extend(vector<hsize_t> size) {
    if (extendable) {
        hsize_t *dsize = &size[0];
        dataset.extend(dsize);
    }
}

//actually, add a new variable: the selected dataspace
//in write, we need both (look into this)
H5::DataSpace node::select(vector<hsize_t> size, vector<hsize_t> offset) {
    //DataSpace fspace;               //or return this instead
    hsize_t *dsize = &size[0];
    hsize_t *doffset = &offset[0];
    dataspace.selectHyperslab(H5_SELECT_SET, dsize, doffset);   //maybe this is where dataspace is useful

    return dataspace;
}


void node::write(double *data) {
    dataset.write(
    
