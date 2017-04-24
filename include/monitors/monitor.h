#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <string>
#include <memory>
#include "../vec.h"
#include "freq.h"
#include "h5cpp.h"


namespace qbox {

    class Field2D;

    //*** technically, these are DFT monitors. Normal monitor can exist too.

    //monitor base class
    class monitor {

    public:
        monitor(std::string name, std::string sub_name, const freq_data &freq, bool extendable): name(name), sub_name(sub_name), freq(freq), extendable(extendable) {};
        monitor(std::string sub_name, const freq_data &freq, bool extendable): monitor("monitor_" + std::to_string(_num_created), sub_name, freq, extendable) {_num_created++;}

        //*** should probably be private:
        virtual void set_F(Field2D *newF);       //set the owning field
        virtual void update() = 0;                //update the DFT values
        virtual Array compute_flux() const = 0;
        void write_flux() const;

        h5cpp::h5group get_group() const;

        monitor() = default;
        monitor(const monitor&) = default;
        monitor(monitor&&) = default;
        monitor& operator=(monitor&&) = default;

    protected:
        std::string name;    //Monitor name; to call output from main
        std::string sub_name;    //name of monitor type (subgroup inside monitors)
        freq_data freq;    //set of frequencies to DFT at
        bool extendable;
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;

    private:
        static int _num_created;
    };

}

#endif
