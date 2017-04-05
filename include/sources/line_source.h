#ifndef GUARD_line_source_h
#define GUARD_line_source_h

#include "source.h"

namespace qbox {

    class line_source: public source {
    public:
        line_source(std::string name, fields C, const surface &surf, const time_profile &tp);
        line_source(fields C, const surface &surf, const time_profile &tp);

        void pulse() override;
        void write() override;

    private:
        fields C;
        surface surf;    //vector position
    };
}

#endif
