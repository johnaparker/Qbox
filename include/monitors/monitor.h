#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <vector>
#include <string>
#include <memory>
#include "../matrix.h"
#include "freq.h"
#include "h5cpp.h"


namespace qbox {

    class Field2D;

    //*** technically, these are DFT monitors. Normal monitor can exist too.

    //monitor base class
    class monitor {

    public:
        monitor(std::string name, std::shared_ptr<freq_data> freq, int N, bool extendable): name(name), N(N), freq(freq), extendable(extendable) {};
        virtual void set_freq(std::shared_ptr<freq_data> new_freq); //set the frequencies
        //*** should probably be private:
        virtual void set_F(Field2D *newF);       //set the owning field
        virtual void update() = 0;                //update the DFT values
        virtual std::unique_ptr<double[]> compute_flux() const = 0;

        h5cpp::h5group get_group();

        monitor() = default;
        monitor(const monitor&) = default;
        monitor(monitor&&) = default;
        monitor& operator=(monitor&&) = default;
    protected:
        std::string name;    //Monitor name; to call output from main
        int N;    //Number of points in the monitor
        std::shared_ptr<freq_data> freq;    //set of frequencies to DFT at
        bool extendable;
        std::unique_ptr<double[]> prevE;   //previous E at all all points in the monitor; needed to interpolate E in time domain
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;
    };


    int get_direction(std::vector<int> p1, std::vector<int> p2);

}
#endif
