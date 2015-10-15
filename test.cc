#include <iostream>
#include "H5Cpp.h"

using namespace std;
using namespace H5;

int main() {
    const int rank = 2;
    const H5std_string file_name("out.h5");
    const H5std_string dataset_name("test");
    const int NX = 5;
    const int NY = 5;
    
    hsize_t dims[2] = {3,3};
    hsize_t maxdims[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    DataSpace mspace1(rank, dims, maxdims);

    H5File file(file_name, H5F_ACC_TRUNC);

    DSetCreatPropList cparms;
    hsize_t chunk_dims[2] = {3,1};
    cparms.setChunk(rank, chunk_dims);
    double fill_val = 0;
    cparms.setFillValue(PredType::NATIVE_DOUBLE, &fill_val);

    DataSet dataset = file.createDataSet(dataset_name, PredType::NATIVE_DOUBLE, mspace1, cparms);
    hsize_t size[2] = {3,3};
    dataset.extend(size);

    DataSpace fspace1 = dataset.getSpace();
    hsize_t offset[2] = {0,0};
    hsize_t dims1[2] = {3,3};
    fspace1.selectHyperslab(H5S_SELECT_SET, dims1, offset);
    double data1[3][3] = {{1,1,1}, {1,1,1}, {1,1,1}};
    dataset.write(data1, PredType::NATIVE_DOUBLE, mspace1, fspace1); 

    size[0] = 5;
    size[1] = 5;
    dataset.extend(size);

      int i, j;
      double data_out[NX][NY];
      for (i = 0; i < NX; i++)
      {
         for (j = 0; j < NY; j++)
            data_out[i][j] = 0;
      }
      dataset.read( data_out, PredType::NATIVE_DOUBLE );
       for (i=0; i < NX; i++)
      {
          for(j=0; j < NY; j++)
             cout << data_out[i][j] << "  ";
          cout << endl;
      }     
    
}
