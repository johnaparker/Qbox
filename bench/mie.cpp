#include <meep.hpp>
#include "h5cpp.h"
#include <vector>

using namespace meep;

double radius = 20;
int Nfreq = 200;

double eps(const vec &p) {
  if (pow(p.x()-60,2) + pow(p.y()-60,2) < pow(radius,2))
    return 2.0;
  return 1.0;
}

double vaccum(const vec&p) {return 1.0;}

int main(int argc, char **argv) {
  initialize mpi(argc, argv); // do this even for non-MPI Meep
  double resolution = 2; // pixels per distance
  grid_volume v = vol2d(120,120, resolution); // 5x10 2d cell
  structure s(v, vaccum, pml(10));
  structure sg(v, eps, pml(10));

  fields f(&s);
  f.use_real_fields();
  fields g(&sg);
  g.use_real_fields();
  
  //f.output_hdf5(Dielectric, v.surroundings());
  
  double freq = 2/30.0, fwidth = 3/30.0;
  gaussian_src_time src(freq, fwidth);
  f.add_volume_source(Ez, src, volume(vec(0,30), vec(120,30)));
  g.add_volume_source(Ez, src, volume(vec(0,30), vec(120,30)));


  auto inc = f.add_dft_flux_plane(volume(vec(50,60), vec(60,60)), 1/30.0, 3/30.0, Nfreq);
  auto box_inc = f.add_dft_flux_box(volume(vec(40,40), vec(80,80)), 1/30.0, 3/30.0, Nfreq);
  auto box_scat = g.add_dft_flux_box(volume(vec(40,40), vec(80,80)), 1/30.0, 3/30.0, Nfreq);

  for (int i = 0; i != 4000; i++) {
    f.step();
  }

  box_scat -= box_inc;

  for (int i = 0; i != 4000; i++) {
    g.step();
  }

  h5cpp::h5file io2("test2.h5", h5cpp::io::w);
  double* S1 = inc.flux();
  double* S2 = box_inc.flux();
  double* S3 = box_scat.flux();

  auto dset = io2.create_dataset("inc", h5cpp::dtype::Double, h5cpp::dspace(std::vector<hsize_t>{Nfreq}));
  dset.write(S1);
  dset = io2.create_dataset("box_inc", h5cpp::dtype::Double, h5cpp::dspace(std::vector<hsize_t>{Nfreq}));
  dset.write(S2);
  dset = io2.create_dataset("box_scat", h5cpp::dtype::Double, h5cpp::dspace(std::vector<hsize_t>{Nfreq}));
  dset.write(S3);
  
  return 0;
} 
