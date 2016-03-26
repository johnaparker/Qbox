#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <vector>
#include <string>
#include "matrix.h"
#include "field2.h"

class Field2D;

//*** technically, these are DFT monitors. Normal monitor can exist too.

//monitor base class
class monitor {
public:
    monitor() = default;
    monitor(std::string name, double *freq, int N): name(name), freq(freq), N(N) {};
    virtual void set_freq(double *new_freq); //set the frequencies
    //*** should probably be private:
    virtual void set_F(Field2D *newF);       //set the owning field
    virtual void update() {};                //update the DFT values

public:
    std::string name;    //Monitor name; to call output from main
    int N;    //Number of points in the monitor
    //*** smart pointers (rule of zero)
    double *freq;    //set of frequencies to DFT at
    double *prevE;   //previous E at all all points in the monitor; needed to interpolate E in time domain
    Field2D *F;      //pointer to owning field object
};

//*** add friendship to Field2D
//*** note that this computes flux in the +x/y direction, but could be smarter to do p/m
//*** or get E/B values from base class
//*** Perhaps allow a res parameter to "skip" some grid cells 
//surface monitor: monitors all points inside a plane
class surface_monitor: public monitor {
public:
    //*** replace vectors with vol here
    //various ways to construct. p1,p2 = corners. 
    surface_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double *freq, int N);   //using freq data
    surface_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N);   //using N points between fmin and fmax
    surface_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f);    //at a single frequency
    surface_monitor() = default;
    
    void update();   //update the DFT matrices
    void set_F(Field2D *newF);    //set ownership
    //*** smart pointer here for S probably
    void compute_flux(double *S); //compute flux though face
    //*** should be through IO class. See field2.h
    void write(std::string filename, bool extendable = false);   //write to filename. Extendable=True means to append, and not overwrite

public:
    std::vector<int> p1g, p2g;        //grid points corners
    std::vector<int> p1, p2;          //physical corners
    matrix<double> rE, iE, rH, iH;    //DFT matrices
    int dir;    //orientation
    int length; //length of monitor in grid points
    //*** move these into base class. Find out a way to not compute 4x
    double *sinf, *cosf;  //sin(f*t) and cos(f*t) stored for efficieny
};

//*** There is memory redunancy here....is it worth it for using a templated add_monitor function instead???
//box monitor: monitors all points inside 4 surface monitors
class box_monitor: public monitor {
public:
    //various constructors
    box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double *freq, int N);
    box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N);
    box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f);
    box_monitor() = default;
    
    //same as surface_monitor 
    void set_freq(double *new_freq);
    void set_F(Field2D *newF);
    void update();
    void compute_flux(double *S);
    void write(std::string filename, bool extendable = false);

    void write_sides(std::string filename, bool extendable = false);  //call write for all surface_monitors

public:
    surface_monitor monitors[4];    //4 surface monitors
};



#endif
