#ifndef GUARD_time_monitor_h
#define GUARD_time_monitor_h

#include <string>
#include <memory>
#include "h5cpp.h"

namespace qbox {

    class Field2D;
    enum class fields;

    class time_monitor {

    public:
        time_monitor(std::string name): name(name) {};

        virtual void write(fields A) const = 0;

        virtual void set_F(Field2D *newF);        //set the owning field
        h5cpp::h5group get_group() const;

    protected:
        std::string name;    //Monitor name; to call output from main
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;

        static int _num_created;
    };

}

#endif
