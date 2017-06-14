#ifndef GUARD_surface_monitor_h
#define GUARD_surface_monitor_h

#include <string>
#include <memory>
#include "../vec.h"
#include "monitor.h"


namespace qbox {

    class Field2D; 

    //*** add friendship to Field2D
    //*** note that this computes flux in the +x/y direction, but could be smarter to do p/m
    //*** or get E/B values from base class
    //*** Perhaps allow a res parameter to "skip" some grid cells 
    //surface monitor: monitors all points inside a plane
    class surface_monitor: public monitor {

    public:
        surface_monitor() = default;
        surface_monitor(std::string name, const surface &surf, const freq_data &freq, bool extendable=false);   //using freq data
        surface_monitor(const surface &surf, const freq_data &freq, bool extendable=false);

        void set_F(Field2D *newF);    //set ownership
        void update();   //update the DFT matrices
        Array compute_flux() const; //compute flux though face

        //equivalent currents
        ComplexArray ntff(const vec &center, const vec &p) const;
        //ComplexTensor Jeq() const;
        //ComplexTensor Meq() const;

        void operator-=(const surface_monitor& other) {
            rE -= other.rE;
            iE -= other.iE;
            rH -= other.rH;
            iH -= other.iH;
        }

    private:
        surface surf;
        Array prevE;     ///< previous electric field values
        int length;      ///< length of monitor in grid points
    };

}

#endif
