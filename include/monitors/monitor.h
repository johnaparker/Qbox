#ifndef GUARD_monitor_h
#define GUARD_monitor_h

#include <string>
#include <memory>
#include "../vec.h"
#include "h5cpp.h"


namespace qbox {

    class Field2D;

    //*** technically, these are DFT monitors. Normal monitor can exist too.

    //monitor base class
    class monitor {

    public:
        monitor(std::string name, std::string sub_name): name(name), sub_name(sub_name) {};

        //*** should probably be private:
        virtual void set_F(Field2D *newF);        //set the owning field
        virtual void update() = 0;                //update the DFT values

        monitor() = default;
        monitor(const monitor&) = default;
        monitor(monitor&&) = default;
        monitor& operator=(monitor&&) = default;

        h5cpp::h5group get_group() const;

    protected:
        std::string name;    //Monitor name; to call output from main
        std::string sub_name;    //name of monitor type (subgroup inside monitors)
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;

        static int _num_created;
    };

}

#endif
