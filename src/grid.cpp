#include "grid.h"

using namespace std;

namespace qbox {

    grid_properties::grid_properties(int Lx, int Ly, int res, int pml_thickness):
            Lx(Lx), Ly(Ly), res(res), pml_thickness(pml_thickness) {
        Nx = Lx*res;
        Ny = Ly*res;
        dx = 1.0/res;
        totalFieldScatteredField = false;       
    }

    void grid_properties::set_tfsf(ivec p1_val, ivec p2_val){
        p1 = convertToGrid(p1_val);
        p2 = convertToGrid(p2_val);
        totalFieldScatteredField = true;
    }

    void grid_properties::set_tfsf(int xbuff, int ybuff){
        ivec p1_val = {xbuff, ybuff};
        ivec p2_val = {Lx-xbuff, Ly-ybuff};
        set_tfsf(p1_val, p2_val);
    }

    void grid_properties::set_tfsf(int buff){
        set_tfsf(buff, buff);
    }

    ivec grid_properties::convertToGrid(const ivec &p) {
        ivec pi = {p[0]*res, p[1]*res};
        return pi;
    }

    ivec grid_properties::convertToReal(const ivec &pi) {
        ivec p = {pi[0]/res, pi[1]/res};
        return p;
    }

}
