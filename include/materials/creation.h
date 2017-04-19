#ifndef GUARD_creation_h
#define GUARD_creation_h

#include "debye.h"
#include "drude.h"
#include "lorentz.h"

namespace qbox {
    lorentz drude_lorentz(const drude& drude_material, const lorentz& lorentz_material);
}

#endif
