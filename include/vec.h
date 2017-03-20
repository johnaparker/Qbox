#ifndef GUARD_vec_h
#define GUARD_vec_h

#include <iostream>
#include <vector>
#include <math.h>
#include <memory>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

namespace qbox {
    using vec = Eigen::Vector2d;
    using veci = Eigen::Vector2i;
    using quat = Eigen::Quaterniond;


    struct vol {
        vol(vec a, vec b): a(a), b(b) {
            dim = (a-b).cwiseAbs();
        }

    public:
        vec a,b;
        vec dim;
    };

}

#endif
