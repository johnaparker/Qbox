#include <qbox.h>
#include <iostream>

using namespace std;
using namespace qbox;

int main() {
    int pml_thickness = 7;
    int res = 2;
    double Lx = 100;
    double Ly = 100;
    double f = 2/30.0;
    vec center{Lx/2,Ly/2};

    grid_properties grid(Lx,Ly,res,pml_thickness);

    Field2D test(grid, "out.h5");
    test.set_tfsf(volume(center, 80), continuous_time(f));
    

    vec orient(1,1);
    //object obj(ellipse(5,10), center, orient); 
    object obj(block(vec(10,20)), center, orient); 
    test.add_object(obj, simple_material(5,1,10));

    cout << grid.Nx << endl;
    //*** grid.volume() should return a volume that guarentees this works
    //*** also have a tfsf.volume()
    //*** also have a grid.no_pml_volume()
    volume_monitor m1("vol", grid.domain(), freq_data(f)); 
    test.add_monitor(m1);


    for (int i = 0; i != 2000; i++) {
        test.update();
        test.writeE();
    }
    m1.write_dft();
}
