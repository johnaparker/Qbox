/* A test to measure the effectiveness of the PML.
   Point monitors are placed in a small grid and a large grid.
   The small grid will include "immediate" PML reflections.
   The large grid sees the reflection at a much later time.
   A relative error can thus be obtained.

   (see Taflove, p.313, Third ed.)
*/

#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 10;
    int res = 1;

    double f = 2/30.0;
    auto source = gaussian_time(f, 1/100.0, 80);

    double Dx, Dy;
    {
        double Lx = 40;
        double Ly = 40;
        grid_properties grid(Lx,Ly,res,pml_thickness);
        Field2D test(grid, "small.h5");
        point_source s1(fields::Ez, {Lx/2,Ly/2}, source);
        test.add_source(s1);

        Dx = Lx/2 - (pml_thickness + 3);
        Dy = Ly/2 - (pml_thickness + 3);

        point_time_monitor m1("A", vec{Lx/2 - Dx, Ly/2});
        point_time_monitor m2("B", vec{Lx/2 - Dx, Ly/2 - Dy});
        test.add_monitor(m1);
        test.add_monitor(m2);
        for (int i = 0; i != 1000; i++) {
            test.update();
            m1.write(fields::Ez);
            m2.write(fields::Ez);
        }
    }

    {
        double Lx = 300;
        double Ly = 300;
        grid_properties grid(Lx,Ly,res,pml_thickness);
        Field2D test(grid, "large.h5");
        point_source s1(fields::Ez, {Lx/2,Ly/2}, source);
        test.add_source(s1);

        point_time_monitor m1("A", vec{Lx/2 - Dx, Ly/2});
        point_time_monitor m2("B", vec{Lx/2 - Dx, Ly/2 - Dy});
        test.add_monitor(m1);
        test.add_monitor(m2);
        for (int i = 0; i != 1000; i++) {
            test.update();
            m1.write(fields::Ez);
            m2.write(fields::Ez);
        }
    }
}
