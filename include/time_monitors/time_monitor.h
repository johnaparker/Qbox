#ifndef GUARD_time_monitor_h
#define GUARD_time_monitor_h

#include <string>
#include <memory>
#include "h5cpp.h"
#include <unordered_map>

namespace qbox {

    class Field2D;
    enum class fields;

    class time_monitor {

    public:
        time_monitor(std::string name): name(name) {};

        void write(const fields& A);

        virtual void set_F(Field2D *newF);        //set the owning field
        h5cpp::h5group get_group() const;

    protected:
        virtual void write_impl(const fields& A) = 0;

    protected:
        std::string name;    //Monitor name; to call output from main
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;
        std::unordered_map<fields, h5cpp::h5dset> dsets;

        static int _num_created;
    };

}

#endif
