#include "matrix.h"
#include "field2.h"
#include "field.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

tfsf::tfsf(grid_properties grid, double dt) {
    ia = grid.p1[0];
    ib = grid.p2[0];
    ja = grid.p1[1];
    jb = grid.p2[1];

    inc = new Field1D(grid.Ny, grid.dx, dt);
}
