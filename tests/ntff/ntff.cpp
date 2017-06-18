#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 7;
    int res = 6;
    double Lx = 100;
    double Ly = 100;

    grid_properties grid(Lx,Ly,res,pml_thickness);

    Field2D test(grid, "out.h5");
    
    double f = 2/30.0;

    vec center{Lx/2,Ly/2};
    vec g{0,0};


    box_monitor<DFT::tangent> m1(volume(center, 40),  Array::LinSpaced(100,1/30.0,3/30.0)); 
    test.add_monitor(m1);

    point_source s1(fields::Ez, center - g, gaussian_time(f, 1/200.0, 80));
    point_source s2(fields::Ez, center + g, gaussian_time(f, 1/200.0, 80));
    test.add_source(s1);
    test.add_source(s2);

    //point_source s1(fields::Ez, center, gaussian_time(f, 1/200.0, 80));
    //test.add_source(s1);

    for (int i = 0; i != 4000; i++) {
        test.update();
        //test.writeE();
    }
    m1.flux().write();
    m1.ntff(100000, 150).write();
}
