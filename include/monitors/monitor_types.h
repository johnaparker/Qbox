#ifndef GUARD_monitor_types_h
#define GUARD_monitor_types_h

#include "rank_monitor.h"

namespace qbox {

    class Flux;

    // Flux monitor: enables flux computation (requires 'tangent' or 'all')
    class flux_monitor: virtual public rank_monitor<1> {
    public:
        virtual Flux flux() const = 0;
    };

    // ntff monitor: enables near-to-far-field computation (requires 'tangent' or 'all')
    class ntff_monitor: virtual public rank_monitor<1> {
    public:
    virtual ComplexArray ntff(const vec &center, const vec &pc) const = 0;
        // other ntff ...
    };

    // Force monitor: enables force and torque calculations (requires 'all')
    class force_monitor: virtual public rank_monitor<1> {
    public:
        virtual Array force() const = 0;
        virtual Array torque() const = 0;
    };

    // Open monitor: Surface does not contain a closed volume (defines flux and ntff)
    class open_monitor: public ntff_monitor, public flux_monitor {
    public:
    };

    // Closed monitor: Surface does contain a closed volume (defines flux, ntff, and force)
    class closed_monitor: public ntff_monitor, public flux_monitor, public force_monitor {
    public:
    };

}

#endif
