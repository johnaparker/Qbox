#ifndef GUARD_point_source_h
#define GUARD_point_source_h

#include "source.h"

namespace qbox {

    class point_source: public source {
    public:
        point_source(std::string name, fields C, vec p, const time_profile &tp);
        point_source(fields C, vec p, const time_profile &tp);

        void pulse() override;
        void write() override;

    private:
        fields C;
        vec p;    //vector position
    };
}

#endif
