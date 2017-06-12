#include "grid.h"
#include "math.h"

using namespace std;

namespace qbox {

    grid_properties::grid_properties(double _Lx, double _Ly, double res, int pml_thickness):
            res(res), pml_thickness(pml_thickness+1) {
        Nx = ceil(_Lx*res);
        Ny = ceil(_Ly*res);
        dx = 1.0/res;
        Lx = dx*(Nx-1);
        Ly = dx*(Ny-1);
        dt = dx/2;
    }

    ivec grid_properties::to_grid(const vec &p) const {
        return Eigen::round(p.array()/dx).cast<int>();
    }

    vec grid_properties::to_real(const ivec &pi) const {
        return dx*pi.cast<double>();
    }

    isurface grid_properties::to_grid(const surface &surf) const {
        return isurface(to_grid(surf.a), to_grid(surf.b), surf.sign);
    }

    surface grid_properties::to_real(const isurface &surf) const {
        return surface(to_real(surf.a), to_real(surf.b), surf.sign);
    }

    ivolume grid_properties::to_grid(const volume &vol) const {
        return ivolume(to_grid(vol.a), to_grid(vol.b));
    }

    volume grid_properties::to_real(const ivolume &vol) const {
        return volume(to_real(vol.a), to_real(vol.b));
    }

    volume grid_properties::clip(const volume &vol) const {
        vec a = vol.a;
        vec b = vol.b;

        if (a(0) < 0)
            a(0) = 0;
        if (b(0) < 0)
            b(0) = 0;
        if (a(1) > Lx)
            a(1) = Lx;
        if (b(1) > Ly)
            b(1) = Ly;

        return volume(a,b);
    }

    ivolume grid_properties::clip(const ivolume &ivol) const {
        ivec a = ivol.a;
        ivec b = ivol.b;

        if (a(0) < 0)
            a(0) = 0;
        if (b(0) < 0)
            b(0) = 0;
        if (a(1) > Nx-1)
            a(1) = Nx-1;
        if (b(1) > Ny-1)
            b(1) = Ny-1;

        return ivolume(a,b);
    }
    //*** other clip functions... and does to_grid etc. clip automatically?


    void grid_properties::write(const h5cpp::h5group &group) {
        h5cpp::write_scalar(Lx, group, "Lx");
        h5cpp::write_scalar(Ly, group, "Ly");
        h5cpp::write_scalar(Nx, group, "Nx");
        h5cpp::write_scalar(Ny, group, "Ny");
        h5cpp::write_scalar(dx, group, "dx");
        h5cpp::write_scalar(dt, group, "dt");
        h5cpp::write_scalar(res, group, "resolution");
        h5cpp::write_scalar(pml_thickness, group, "pml_thickness");
    }

}
