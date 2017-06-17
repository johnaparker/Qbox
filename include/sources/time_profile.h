#ifndef GUARD_time_profile_h
#define GUARD_time_profile_h

#include <memory>
#include <optional>
#include "h5cpp.h"
#include "dft.h"

namespace qbox {

    class time_profile {
    public:
        time_profile(std::optional<Array> freq = std::nullopt);
        // This function must be called before response is returned
        void update(double val, double t);

        virtual double response(double t) = 0;
        virtual void write(const h5cpp::h5group &group) const = 0;
        virtual std::unique_ptr<time_profile> clone() const = 0;
    private:
        std::optional<dft<0>> fourier;
    };

    class gaussian_time: public time_profile {
    public:
        gaussian_time(double f, double sig, double T0);

        double response(double t) override;
        void write(const h5cpp::h5group &group) const override;
        std::unique_ptr<time_profile> clone() const override;

    private:
        double f, sig, T0;
    };

    class continuous_time: public time_profile {
    public:
        continuous_time(double f);
        double response(double t) override;
        void write(const h5cpp::h5group &group) const override;

        std::unique_ptr<time_profile> clone() const override;
    private:
        double f;
    };
}

#endif
