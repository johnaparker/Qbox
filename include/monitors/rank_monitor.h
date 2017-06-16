#ifndef GUARD_rank_monitor_h
#define GUARD_rank_monitor_h

#include "monitor.h"
#include "../dft.h"

namespace qbox {

    template <int RANK>
    class rank_monitor: public monitor {

    public:
        rank_monitor(std::string name, std::string sub_name, const Array &freq): monitor(name, sub_name), fourier(freq) {};
        rank_monitor(std::string sub_name, const Array &freq): monitor(sub_name), fourier(freq) {};

        void operator-=(const rank_monitor& other) {
            fourier -= other.fourier;
        }

        void write() const {
            fourier.write(get_group());
        }

    protected:
        dft<RANK> fourier;
        // std::optional<Eigen::Tensor<double,RANK,Eigen::RowMajor>> prevE;
    };

}

#endif
