#include "H5Cpp.h"
#include <vector>

class node {
public:
    H5::DataSpace data;
    H5::H5std_string name;
public:
    node();
};

class h5out {
public:
    int rank, Nx, Ny;
    H5::hsize_t dims[2], maxdims[2];
     
    H5::H5File outFile;
    H5::H5std_string file_name;
    std::vector<node> nodes;

public:
    h5out();
    void create_node(std::string node_name);
    void write_to_node();
};



    
        

