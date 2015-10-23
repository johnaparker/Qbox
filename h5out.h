#ifndef GUARD_h5out_h
#define GUARD_h5out_h

#include <H5Cpp.h>
#include "matrix.h"
#include <vector>
#include <map>

class node {
public:
    H5std_string name;
    int rank;
    hsize_t *dims;
    H5::DataSpace dataspace;  //I don't think this is necessary
    H5::DataSpace selected;
    H5::DataSet dataset;

    bool extendable;
public:
    node(H5std_string name, H5::H5File & head, std::vector<hsize_t> size, bool extendable = true);
    node() = default;
    void extend(int amount = 1);
    void select(std::vector<hsize_t> size, std::vector<hsize_t> offset);
    void select();
    void write(double *data);
    bool isExtendable();
};

class h5out {
public:
    H5::H5File outFile;
    H5std_string filename;
    std::map<H5std_string, node> nodes;
    std::map<H5std_string, bool> first_write;
public:
    h5out(H5std_string filename);
    h5out() = default;
    void create_node(H5std_string node_name, std::vector<hsize_t> size, bool extendable = true);
    void create_node(H5std_string node_name, matrix<double> & data, bool extendable = true);
    void write_to_node(H5std_string node_name, double *data);
    void write_to_node(H5std_string node_name, matrix<double> & data);
};

#endif

    
        

