#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 20;
    int res = 2;

    grid_properties grid(120,120,res,pml_thickness);

    Field2D test(grid, "out.h5");
    
    double f = 2/30.0;
    auto freq_data = Array::LinSpaced(500, 1/30.0, 3/30.0);
    box_monitor<DFT::tangent> m1("m1", volume({60,60}, 20,20), freq_data);
    box_monitor<DFT::tangent> m2("m2", volume({95,95}, 20,20), freq_data);
    test.add_monitor(m1);
    test.add_monitor(m2);

    point_source s1(fields::Ez, {60,60}, gaussian_time(f, 1/200.0, 80));
    test.add_source(s1);

    for (int i = 0; i != 4000; i++) {
        test.update();
        // test.writeE();
    }

    //*** don't allow this if setF hasn't been called (add_monitor)
    //*** (really though, shouldn't monitors require field to be passed at construction?)
    m1.flux().write();
    m2.flux().write();
    //*** compare this to pre-monitor code ... m2 flux is much larger 
}
