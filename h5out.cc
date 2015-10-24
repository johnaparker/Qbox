#include "h5out.h"
#include <vector>
#include "matrix.h"


using namespace std;
using namespace H5;


node::node(H5std_string name, H5File & head, vector<hsize_t> size, bool extendable):
                                                  name(name), extendable(extendable) {
    rank = size.size();
    double fill_value = 0;
    dims = new hsize_t[rank];
    hsize_t *maxdims = new hsize_t[rank];
    hsize_t *chunk_dims = new hsize_t[rank];
    for (int i = 0; i != rank; i++) {
        dims[i] = size[i];
        maxdims[i] = size[i];
        chunk_dims[i] = size[i];               //check different chunking
    }
    if (extendable)
        maxdims[rank-1] = H5S_UNLIMITED;

    DSetCreatPropList cparms;
    cparms.setChunk(rank, chunk_dims);
    cparms.setFillValue(PredType::NATIVE_DOUBLE, &fill_value);
    
    dataspace = DataSpace(rank, dims, maxdims);
    selected = dataspace;
    dataset = head.createDataSet(name, PredType::NATIVE_DOUBLE, dataspace, cparms);

    delete[] maxdims;
    delete[] chunk_dims;
}

void node::extend(int amount) {
    if (extendable) {
        dims[rank-1] += amount;
        dataset.extend(dims);
    }
}

void node::select(vector<hsize_t> size, vector<hsize_t> offset) {
    hsize_t *dsize = &size[0];
    hsize_t *doffset = &offset[0];
    selected = dataset.getSpace();
    selected.selectHyperslab(H5S_SELECT_SET, dsize, doffset); 
}

void node::select() {
    vector<hsize_t> size(&dims[0], &dims[rank]);
    vector<hsize_t> offset(rank,0);
    if (extendable) {
        size[rank-1] = 1;
        offset[rank-1] = dims[rank-1]-1;
    }
    select(size, offset);
}

void node::write(double *data) {
    dataset.write(data, PredType::NATIVE_DOUBLE, dataspace, selected);
}

bool node::isExtendable() {
    return extendable;
}
    

h5out::h5out(H5std_string filename): filename(filename) {
    outFile = H5File(filename, H5F_ACC_TRUNC);
}

void h5out::create_node(H5std_string node_name, vector<int> size, bool extendable) {
    if (extendable)
        size.push_back(1);
    vector<hsize_t> dsize;
    for (auto const & s: size)
        dsize.push_back((long long unsigned int) s);
    node newNode(node_name, outFile, dsize, extendable);
    nodes[node_name] = newNode;
    first_write[node_name] = true;
}

void h5out::create_node(H5std_string node_name, matrix<double> & data, bool extendable) {
    vector<int> size(data.get_Nx(), data.get_Ny());
    create_node(node_name, size, extendable);
    write_to_node(node_name, data); 
}

void h5out::write_to_node(H5std_string node_name, double *data) {
    if (!first_write[node_name])
        nodes[node_name].extend();
    else (first_write[node_name] = false);
    nodes[node_name].select();
    nodes[node_name].write(data);
}

void h5out::write_to_node(H5std_string name, matrix<double> & data) {
    write_to_node(name, data.data());
}

bool h5out::contains(H5std_string node_name) {
    return nodes.count(node_name);
}
