#ifndef GUARD_cylinder_monitor_h
#define GUARD_cylinder_monitor_h

#include <string>
#include <memory>
#include "matrix.h"
#include "monitor.h"
#include "vec.h"


namespace qbox {

    class Field2D; 

    class cylinder_monitor: public monitor {

    public:
        //various ways to construct. p1,p2 = corners. 
        cylinder_monitor() = default;
        cylinder_monitor(std::string name, const cylinder_surface &surf, const freq_data &freq, bool extendable=false);   //using freq data
        cylinder_monitor(const cylinder_surface &surf, const freq_data &freq, bool extendable=false);   //using freq data

        void set_F(Field2D *newF);    //set ownership
        void update();   //update the DFT matrices
        Array compute_flux() const; //compute flux though face

        void operator-=(const cylinder_monitor& other) {
            rE -= other.rE;
            iE -= other.iE;
            rH -= other.rH;
            iH -= other.iH;
        }

    private:
        bool extendable;
        cylinder_surface surf;
        matrix<double,2> rE, iE, rH, iH;    //DFT matrices
        matrix<double,1> prevE;             ///< previous electric field values
        int length; //length of monitor in grid points
    };

}

#endif
