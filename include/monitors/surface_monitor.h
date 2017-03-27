#ifndef GUARD_surface_monitor_h
#define GUARD_surface_monitor_h

#include <string>
#include <memory>
#include "../matrix.h"
#include "monitor.h"
#include "vec.h"


namespace qbox {

    class Field2D; 

    //*** add friendship to Field2D
    //*** note that this computes flux in the +x/y direction, but could be smarter to do p/m
    //*** or get E/B values from base class
    //*** Perhaps allow a res parameter to "skip" some grid cells 
    //surface monitor: monitors all points inside a plane
    class surface_monitor: public monitor {

    public:
        //*** replace vectors with vol here
        //various ways to construct. p1,p2 = corners. 
        surface_monitor() = default;
        surface_monitor(std::string name, const surface &surf, const freq_data &freq, bool extendable=false);   //using freq data

        void set_F(Field2D *newF);    //set ownership
        void update();   //update the DFT matrices
        Array compute_flux() const; //compute flux though face

    private:
        bool extendable;
        surface surf;
        matrix<double,2> rE, iE, rH, iH;    //DFT matrices
        matrix<double,1> prevE;             ///< previous electric field values
        int dir;    //orientation
        int length; //length of monitor in grid points
    };

}

#endif
