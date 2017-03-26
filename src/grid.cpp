#include "grid.h"
#include "math.h"

using namespace std;

namespace qbox {

    grid_properties::grid_properties(double _Lx, double _Ly, double res, int pml_thickness):
            res(res), pml_thickness(pml_thickness) {
        Nx = ceil(_Lx*res);
        Ny = ceil(_Ly*res);
        dx = 1.0/res;
        Lx = dx*(Nx-1);
        Ly = dx*(Ny-1);
        totalFieldScatteredField = false;       
    }

    void grid_properties::set_tfsf(vec p1_val, vec p2_val){
        p1 = to_grid(p1_val);
        p2 = to_grid(p2_val);
        totalFieldScatteredField = true;
    }

    void grid_properties::set_tfsf(double xbuff, double ybuff){
        set_tfsf({xbuff,ybuff}, {Lx-xbuff, Ly-ybuff});
    }

    void grid_properties::set_tfsf(double buff){
        set_tfsf(buff, buff);
    }

    ivec grid_properties::to_grid(const vec &p) {
        return Eigen::round(p.array()/dx).cast<int>();
    }

    vec grid_properties::to_real(const ivec &pi) {
        return dx*pi.cast<double>();
    }

    isurface grid_properties::to_grid(const surface &surf) {
        return isurface(to_grid(surf.a), to_grid(surf.b), surf.sign);
    }

    surface grid_properties::to_real(const isurface &surf) {
        return surface(to_real(surf.a), to_real(surf.b), surf.sign);
    }

    ivolume grid_properties::to_grid(const volume &vol) {
        return ivolume(to_grid(vol.a), to_grid(vol.b));
    }

    volume grid_properties::to_real(const ivolume &vol) {
        return volume(to_real(vol.a), to_real(vol.b));
    }

}