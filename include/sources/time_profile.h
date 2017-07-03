#ifndef GUARD_time_profile_h
#define GUARD_time_profile_h

#include <memory>
#include <optional>
#include "h5cpp.h"
#include "../dft.h"

namespace qbox {

    class time_profile {
    public:
        // This function must be called before response is returned
        void update(double val, double t);

        virtual double response(double t) = 0;
        virtual void write(const h5cpp::h5group &group) const = 0;
        virtual std::unique_ptr<time_profile> clone() const = 0;

        std::optional<dft<0>> get_dft() const {return fourier;}

    protected:
        std::optional<dft<0>> fourier;
    };

    template<class BASE>
    class time_profile_inter: public time_profile {
    public:
        BASE & freq(const Array &freq) {
            fourier = dft<0>(freq, "dft", {});
            return *dynamic_cast<BASE*>(this);
        }

        virtual double response(double t) = 0;
        virtual void write(const h5cpp::h5group &group) const = 0;
        virtual std::unique_ptr<time_profile> clone() const = 0;
    };

    class gaussian_time: public time_profile_inter<gaussian_time> {
    public:
        gaussian_time(double f, double sig, double T0);

        double response(double t) override;
        void write(const h5cpp::h5group &group) const override;
        std::unique_ptr<time_profile> clone() const override;

    private:
        double f, sig, T0;
    };

    class continuous_time: public time_profile_inter<continuous_time> {
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
