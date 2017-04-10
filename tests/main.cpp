#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 10;
    int res = 2;
    double Lx = 120;
    double Ly = 120;


    grid_properties grid(Lx,Ly,res,pml_thickness);

    Field2D test(grid, "out.h5");
    
    double f = 2/30.0;

    point_source s1(fields::Ez, {30,30}, gaussian_time(f, 1/200.0, 80));
    test.add_source(s1);

    for (int i = 0; i != 3000; i++) {
        test.update();
        test.writeE();
    }
    //m1.write_flux_sides();
}
