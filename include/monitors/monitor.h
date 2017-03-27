#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <string>
#include <memory>
#include "../matrix.h"
#include "freq.h"
#include "h5cpp.h"
#include "vec.h"


namespace qbox {

    class Field2D;

    //*** technically, these are DFT monitors. Normal monitor can exist too.

    //monitor base class
    class monitor {

    public:
        monitor(std::string name, const freq_data &freq, bool extendable): name(name), freq(freq), extendable(extendable) {};
        virtual void set_freq(const freq_data &new_freq); //set the frequencies
        //*** should probably be private:
        virtual void set_F(Field2D *newF);       //set the owning field
        virtual void update() = 0;                //update the DFT values
        virtual Eigen::ArrayXd compute_flux() const = 0;
        void write_flux();

        h5cpp::h5group get_group();

        monitor() = default;
        monitor(const monitor&) = default;
        monitor(monitor&&) = default;
        monitor& operator=(monitor&&) = default;

    protected:
        std::string name;    //Monitor name; to call output from main
        freq_data freq;    //set of frequencies to DFT at
        bool extendable;
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;
    };

}

#endif
