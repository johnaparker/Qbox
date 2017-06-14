#ifndef GUARD_cylinder_monitor_h
#define GUARD_cylinder_monitor_h

#include <string>
#include <memory>
#include "monitor.h"
#include "../vec.h"


namespace qbox {

    class Field2D; 

    class cylinder_monitor: public monitor {

    public:
        //various ways to construct. p1,p2 = corners. 
        cylinder_monitor() = default;
        cylinder_monitor(std::string name, const cylinder_surface &surf, const Array &freq);
        cylinder_monitor(const cylinder_surface &surf, const Array &freq);

        void set_F(Field2D *newF);    //set ownership
        void update();   //update the DFT matrices
        Array compute_flux() const; //compute flux though face

        void operator-=(const cylinder_monitor& other) {
            fourier -= other.fourier;
        }

    private:
        cylinder_surface surf;
        Array prevE;  ///< previous electric field values
        Array freq;
        dft<1> fourier;
        int length;   ///< length of monitor in grid points
    };

}

#endif
