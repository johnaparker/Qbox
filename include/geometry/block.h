#ifndef GUARD_block_h
#define GUARD_block_h

#include "geometry/geometry.h"

namespace qbox {

    class block: public geometry {
    public:
        block(vec dim);
        bool inside(const vec& v) const override;
        void write(const h5cpp::h5group& group) const override;
        std::unique_ptr<geometry> clone() const override;

        std::string group_name() const override {return "blocks";}

    private:
        vec dim;
    };

}

#endif
