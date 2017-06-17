#ifndef GUARD_flux_h
#define GUARD_flux_h

#include <string>
#include "h5cpp.h"
#include "../vec.h"

namespace qbox {

    class Flux {
    public:
        Flux(const Array& S, const h5cpp::h5file &file, const h5cpp::h5group &group );
        Flux(const Array& S, const std::string &file_name, const std::string &group_path);
        void write();

        Array flux() {return S;}

    private:
        Array S;
        std::string file_name;
        std::string group_path;
    };

}

#endif
