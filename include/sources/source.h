#ifndef GUARD_source_h
#define GUARD_source_h

#include "vec.h"

//*** Think about whether this is the best way to do sources
//*** Also think about how to add TFSF into these sources
//*** Lastly, the best way to couple these sources with the field class.

//*** Definitely consider functions external to classes doing some of the work
//*** use "=0" on virtual base, and add virtual destructor

//*** Point source = line source default with one point?

//***the virtual function should be an amplitude call, pulse should be
//   non-virtual, and *F should be private

namespace qbox {

    class Field2D;

    //source base class
    class source {
    public:
        source();
        void set_F(Field2D *F);   //set field ownership
        virtual void pulse() = 0;  //add the source to the fields

    protected:
        Field2D *F;    //field owner reference
        double *t;     //current time (from F)
    };
}

#endif
