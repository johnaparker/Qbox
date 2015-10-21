#include "H5Cpp.h"
#include "matrix.h"
#include <vector>
#include <map>

//support for matrix class
//consider writing it so that only time dim is extendable

class node {
public:
    int rank;
    double fill_value;
    H5::hsize_t *dims, *maxdims;
    H5::DataSpace dataspace;  //I don't think this is necessary
    H5::H5std_string name;
    H5::DataSet dataset;

    bool extendable;
public:
    node(H5::H5std_string name, gH5::H5File & head, std::vector<H5::hsize_t> size, std::vector<H5::hsize_t> maxsize);
    void extend(std::vector<H5::hsize_t> size);
    H5::DataSpace select(std::vector<H5::hsize_t> size, std::vector<H5::hsize_t> offset);
    void write(double *data, H5::DataSpace fspace);
};

class h5out {
public:
    H5::H5File outFile;
    H5::H5std_string file_name;
    std::map<H5std_string, node> nodes;
public:
    h5out();
    void create_node(H5::H5std_string node_name, estd::vector<H5::hsize_t> size, std::vector<H5::hsize_t> maxsize);
    void write_to_node(H5::H5std_string, double *data, std::vector<H5::hsize_t> size, std::vector<H5::hsize_t> offset);
    void write_to_node(H5::H5std_string, matrix & data);
};



    
        

