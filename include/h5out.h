/**
 *  @file
 *  This is a thin wrapper for the C++ HDF5 interface. Support for MPI 
 *  cannot exist with the C++ version.
 *  
 *  h5out: Handles a single HDF5 file. There exists multiple nodes that 
 *  can be written to. \n
 *  node: Interfaces with HDF5 to write to a given dataspace inside an
 *  HDF5 file.
 *  
 *  This wrapper allows an unlimited number of nodes, with arbitrary
 *  rank/dimension size. Nodes are either extendable or not extendable;
 *  extendable means that the last dimension can be appended to "in
 *  time". This is useful for data of fixed size being written to
 *  repeatedly.
*/

#ifndef GUARD_h5out_h
#define GUARD_h5out_h

#include <H5Cpp.h>
#include <vector>
#include <map>
#include <memory>
#include "matrix.h"

namespace qbox {

    /// Node of an h5out object
    /**
     *  A node is a data member of an HDF5 file. It provides an interface
     *  for writing and extending that data member
     */
    class node {
    public:
        /// Construct the node
        /**
         *  This creates a new node in an existing h5out object
         *  @param name The name of the data member
         *  @param head The H5file that the data member belongs to
         *  @param size Vector containing the dimension (\c len(size)) 
         *              and size of each dimension (\c size elements)
         *  @param extendable Boolean value for whether the node can be extended
         */
        node(H5std_string name, H5::H5File & head, std::vector<hsize_t> size, bool extendable = true);
        node() = default;
        node(node&&) = default;
        node& operator= (node&&) = default;

        /// Extend the final dimension size by amount, if node is extendable
        void extend(int amount = 1);

        /// @brief Select hyperslab
        /// @param size Vector of size/dimensionality of the hyperslab
        /// @param offset Vector of offset from 1st element
        void select(std::vector<hsize_t> size, std::vector<hsize_t> offset);

        /// Select the default hyperslab (most recent)
        void select();

        /// @brief Write new data (automatically extends)
        /// @param data Pointer to data to be written
        /// @todo double* data should be passed in some other way
        void write(const double* data);

        /// Returns true if extendable
        bool isExtendable();

    private:
        H5std_string name;       ///< name of node
        int rank;                ///< rank = dimensionality
        std::unique_ptr<hsize_t[]> dims;     ///< array of size rank that contains the size of each dimension

        H5::DataSpace dataspace;  ///< I don't think this is necessary
        H5::DataSpace selected;   ///< The currently selected hyperslab
        H5::DataSet dataset;      ///< Dataset

        bool extendable;          ///< True if dataset is extendable (default)
    };

    /// h5out object maintains an HDF5 file
    /**
     *  Contains a list of nodes (data members inside the file).
     *  New nodes can be creates, and existing nodes can be written to.
     */
    class h5out {
    public:
        /// Create h5out with given filename
        h5out(H5std_string filename);
        h5out() = default;

        /// @brief Create a new node
        /// @param node_name Name of the data member (node)
        /// @param size Vector containing the dimension (\c len(size)) 
        ///              and size of each dimension (\c size elements)
        /// @param extendable Boolean value for whether the node can be extended
        void create_node(H5std_string node_name, std::vector<int> size, bool extendable = true);

        /// Create a new node
        /**
         *  This calls the standard create_node function, but vector<int> size is inferred
         *  from matrix \c data. \c data's internal data is not used.
         *  @param node_name Name of the data member (node)
         *  @param data Matrix with the desired size of the new node
         *  @param extendable Boolean value for whether the node can be extended
         *  @todo data should be constant, but check issues
         */
        void create_node(H5std_string node_name, matrix<double> & data, bool extendable = true);

        /// @brief Write data to node
        /// @param node_name Name of the node to write to
        /// @param data Pointer to data to be written
        /// @todo Look into passing other than double*
        void write_to_node(H5std_string node_name, const double* data);

        /// @brief Write matrix data to node
        /// @param node_name Name of the node to write to
        /// @param data Matrix of data to be written
        void write_to_node(H5std_string node_name, const matrix<double> & data);

        /// Returns true if node_name already exists
        bool contains(H5std_string node_name);

    private:
        H5::H5File outFile;         ///< HDF5 file object
        H5std_string filename;      ///< HDF5 filename
        std::map<H5std_string, node> nodes;    ///< map of node_names to nodes
        std::map<H5std_string, bool> first_write;  ///< map of node_names to booleans; True if the first write has yet to happen
    };
}

#endif

    
        

