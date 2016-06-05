/*
This is a thin wrapper for the C++ HDF5 interface. Support for MPI cannot exist with the C++ version.

h5out: Handles a single HDF5 file. There exists multiple nodes that can be written to.
node: Interfaces with HDF5 to write to a given dataspace inside an HDF5 file.

This wrapper allows an unlimited number of nodes, with arbitrary rank/dimension size. Nodes are either
extendable or not extendable; extendable means that the last dimension can be appended to "in time".
This is useful for data of fixed size being written to repeatedly.
*/

#ifndef GUARD_h5out_h
#define GUARD_h5out_h

#include <H5Cpp.h>
#include <vector>
#include <map>
#include "matrix.h"

namespace qbox {

    //*** Rule of Three
    class node {
    public:
        node(H5std_string name, H5::H5File & head, std::vector<hsize_t> size, bool extendable = true);
        node() = default;

        void extend(int amount = 1);  //extend the final dimension size by amount if extendable
        void select(std::vector<hsize_t> size, std::vector<hsize_t> offset);   //select hyperslab
        void select();    //select default hyperslab (most recent)
        void write(double *data);   //write new data (automatically extends)
        bool isExtendable();        //returns True if extendable

    public:
        H5std_string name;       //name of node
        int rank;                //rank = dimensionality
        hsize_t *dims;           //array of size rank that contains the size of each dimension

        H5::DataSpace dataspace;  //I don't think this is necessary
        H5::DataSpace selected;   //The currently selected hyperslab
        H5::DataSet dataset;      //Dataset

        bool extendable;          //True if dataset is extendable (default)
    };

    class h5out {
    public:
        h5out(H5std_string filename);
        h5out() = default;

        //create and write to nodes
        void create_node(H5std_string node_name, std::vector<int> size, bool extendable = true);   //using vector to speciy dimensionality; rank = len(size)
        ///*** data should be constant here
        void create_node(H5std_string node_name, matrix<double> & data, bool extendable = true);   //using a matrix's shape, ignoring its data
        void write_to_node(H5std_string node_name, double *data);   //write raw data
        void write_to_node(H5std_string node_name, matrix<double> & data);  //write matrix data
        bool contains(H5std_string node_name);     //True if node_name already exists

    public:
        H5::H5File outFile;         //HDF5 file object
        H5std_string filename;      //HDF5 filename
        std::map<H5std_string, node> nodes;    //map of node_names to nodes
        std::map<H5std_string, bool> first_write;  //map of node_names to booleans; True if the first write has yet to happen
    };
}

#endif

    
        

