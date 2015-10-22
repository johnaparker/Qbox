#include "h5out.h"
#include <vector>
#include "matrix"


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
        for (int i = 0; i != rank; i++) {
            dims[i] = dsize[i];
        }
    }
}

//actually, add a new variable: the selected dataspace
//in write, we need both (look into this)
void node::select(vector<hsize_t> size, vector<hsize_t> offset) {
    hsize_t *dsize = &size[0];
    hsize_t *doffset = &offset[0];
    selected = dataset.getSpace();
    selected.selectHyperslab(H5_SELECT_SET, dsize, doffset); 
}


void node::write(double *data) {
    dataset.write(data, PredType::NATIVE_DOUBLE, dataspace, selected);
}

bool node::isExtendable() {
    return extendable;
}
    

h5out::h5out(H5std_string filename): filename(filename) {
    nodes = {};
    outFile = H5File(filename, H5F_ACC_TRUNC);
}

//this shouldn't take maxsize, but rather a bool expandable
void h5out::create_node(H5std_string node_name, vector<hsize_t> size, vector<hsize_t> maxsize) {
    node newNode(node_name, size, maxsize);
    nodes[node_name] = newNode;
}

//node class should be friended for this
//this shouldn't take size, but rather a bool expandable
void write_to_node(H5std_string name, double *data, vector<hsize_t> size) {
    int rank = nodes[node_name].rank;
    //check to see if this is the first time writing
    if (nodes[node_name].isExtendable()) {
        vector<hsize_t> extend_size(&nodes[node_name].dims[0], &nodes[node_name].dims[rank-1]); 
        extend_size[rank-1] += 1;
        nodes[node_name].extend(extend_size);
    }

    vector<hsize_t> offset(rank, 0);
    offset[rank-1] = nodes[node_name].dims[rank-1] - 1;
    nodes[node_name].select(size, iffset);

    nodes[node_name].write(data);
}

//this shouldn't take size, but rather a bool expandable
void write_to_node(H5std_string name, matrix & data, vector<hsize_t> size) {
    write_to_node(name, data.data, size);
}


