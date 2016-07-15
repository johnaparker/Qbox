#ifndef GUARD_surface_monitor_h
#define GUARD_surface_monitor_h

#include <vector>
#include <string>
#include <memory>
#include "../matrix.h"
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
        //*** replace vectors with vol here
        //various ways to construct. p1,p2 = corners. 
        surface_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, std::shared_ptr<freq_data> freq, int N, bool extendable=false);   //using freq data
        surface_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N, bool extendable=false);   //using N points between fmin and fmax
        surface_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f, bool extendable=false);    //at a single frequency

        surface_monitor() = default;
        surface_monitor(const surface_monitor&) = default;
        surface_monitor(surface_monitor&&) = default;
        surface_monitor& operator=(surface_monitor&) = delete;
        surface_monitor& operator=(surface_monitor&&) = default; 
        void update();   //update the DFT matrices
        void set_F(Field2D *newF);    //set ownership
        //*** smart pointer here for S probably
        std::unique_ptr<double[]> compute_flux() const; //compute flux though face
        //*** should be through IO class. See field2.h
        void write_flux();   //write to filename. Extendable=True means to append, and not overwrite

    private:
        bool extendable;
        std::vector<int> p1g, p2g;        //grid points corners
        std::vector<int> p1, p2;          //physical corners
        matrix<double> rE, iE, rH, iH;    //DFT matrices
        int dir;    //orientation
        int length; //length of monitor in grid points
        //*** move these into base class. Find out a way to not compute 4x
    };
}

#endif
