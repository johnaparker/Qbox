#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 6;
    int res = 1;

    double Lx = 30;
    double Ly = 240;
    grid_properties grid(Lx,Ly,res,pml_thickness);

    Field2D test(grid, "out.h5");
    
    double f = 2/30.0;

    point_source s1(fields::Ez, {15,15}, continuous_time(f));
    test.add_source(s1);

    //add_point_time_monitor(test, vec{1,30}, fields::Ez, every(1));

    point_time_monitor m1("left", vec{1,Lx/2});
    point_time_monitor m2("right", vec{Lx-2,Ly/2});
    point_time_monitor m3("bottom", vec{Lx/2,1});
    point_time_monitor m4("top", vec{Lx/2,Ly-2});
    point_time_monitor m5("center", vec{Lx/2,Ly/2});
    surface_time_monitor m6("left_line", surface({1,1}, {1,Ly-2}));
    surface_time_monitor m7("top_line", surface({1,Ly-2}, {Lx-2,Ly-2}));
    test.add_monitor(m1);
    test.add_monitor(m2);
    test.add_monitor(m3);
    test.add_monitor(m4);
    test.add_monitor(m5);
    test.add_monitor(m6);
    test.add_monitor(m7);

    for (int i = 0; i != 100000; i++) {
        test.update();
        m1.write(fields::Ez);
        m2.write(fields::Ez);
        m3.write(fields::Ez);
        m4.write(fields::Ez);
        m5.write(fields::Ez);
        m6.write(fields::Ez);
        m7.write(fields::Ez);
        if (i%100 == 0)
            test.writeE();
    }
}
