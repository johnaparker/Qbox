#include "tfsf.h"
#include "matrix.h"
#include "field2.h"
#include "field.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

//best way to implement this into sources. 

namespace qbox {

    tfsf::tfsf(const grid_properties &grid, const time_profile &tp, const volume &vol, double dt): tp(tp.clone()) {
        auto ivol = grid.to_grid(vol);
        ia = ivol.a[0];
        ib = ivol.b[0];
        ja = ivol.a[1];
        jb = ivol.b[1];

        inc = make_unique<Field1D>(grid.Ny, grid.dx, dt);
    }

    void tfsf::pulse() {
        inc->pulse(*tp);
        inc->update();
    }

    void tfsf::updateD(Field2D* f) {
        for (int i = ia; i <= ib; i++) {
            f->Dz(i,ja) += 0.5*inc->Hx[ja-1];
            f->Dz(i,jb) += -0.5*inc->Hx[jb];
        }
    }

    void tfsf::updateH(Field2D* f) {
        for (int i = ia; i <= ib; i++) {
            f->Hx(i,ja-1) += 0.5*inc->Ez[ja];
            f->Hx(i,jb) += -0.5*inc->Ez[jb];
        }
        for (int j = ja; j <= jb; j++) {
            f->Hy(ia-1,j) += -0.5*inc->Ez[j];
            f->Hy(ib,j) += 0.5*inc->Ez[j];
        }
    }

}
