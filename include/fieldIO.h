#ifndef GUARD_fieldIO_h
#define GUARD_fieldIO_h

#include <memory>
#include <string>
#include "h5cpp.h"
#include "field2.h"

namespace qbox {

    enum class fields {
        Ez,
        Hx,
        Hy
    };

    class fieldIO {
    public:
        fieldIO(std::string filename, Field2D* fp);
        void write_field(const fields field); 
        void write_monitor(std::string name, double *data, int N, bool extendable = false);   //write a monitor

    private:
        void create_fields_dataset(fields field);
        std::string filename;
        Field2D* fp;
        std::unique_ptr<h5cpp::h5file> outFile;
    };
}

#endif
