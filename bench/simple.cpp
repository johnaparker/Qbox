#include <meep.hpp>
#include "h5cpp.h"
#include <vector>

using namespace meep;

double eps(const vec &p) {
  //if (p.x() < 2 && p.y() < 3)
    //return 12.0;
  return 1.0;
}

int main(int argc, char **argv) {
  initialize mpi(argc, argv); // do this even for non-MPI Meep
  double resolution = 2; // pixels per distance
  grid_volume v = vol2d(120,120, resolution); // 5x10 2d cell
  structure s(v, eps, pml(10));
  fields f(&s);
  f.use_real_fields();
  
  //f.output_hdf5(Dielectric, v.surroundings());
  
  double freq = 2/30.0, fwidth = 3/30.0;
  gaussian_src_time src(freq, fwidth);
  f.add_point_source(Ez, src, vec(60, 60));


  int Nfreq = 500;
  auto b1 = f.add_dft_flux_box(volume(vec(50,50), vec(70,70)), 1/30.0, 3/30.0, Nfreq);
  auto b2 = f.add_dft_flux_box(volume(vec(80,80), vec(100,100)), 1/30.0, 3/30.0, Nfreq);

  //h5file io("test.h5", h5file::WRITE);
  for (int i = 0; i != 4000; i++) {
    f.step();
    //f.output_hdf5(Ez, v.surroundings(), &io, true);
  }

  h5cpp::h5file io2("test.h5", h5cpp::io::w);
  double* S1 = b1.flux();
  double* S2 = b2.flux();
  auto dset = io2.create_dataset("S1", h5cpp::dtype::Double, h5cpp::dspace(std::vector<hsize_t>{Nfreq}));
  dset.write(S1);
  dset = io2.create_dataset("S2", h5cpp::dtype::Double, h5cpp::dspace(std::vector<hsize_t>{Nfreq}));
  dset.write(S2);
  
  
  
  return 0;
} 
