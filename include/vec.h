#ifndef GUARD_vec_h
#define GUARD_vec_h

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <stdexcept>

namespace qbox {

    using vec = Eigen::Vector2d;
    using ivec = Eigen::Vector2i;
    using quat = Eigen::Quaterniond;


    struct volume {
        volume(vec a, vec b): a(a), b(b) {
            dim = (a-b).cwiseAbs();
        }

        volume(vec center, double lx, double ly): dim(vec(lx,ly)) {
            a = center - vec(lx,ly)/2;
            b = center + vec(lx,ly)/2;
        }

        volume(vec center, double l): volume(center,l,l) {};

    public:
        vec a,b;
        vec dim;
    };

    struct surface {
        surface(vec a, vec b, int sign = 1): a(a), b(b) {
            dim = (a-b).cwiseAbs();

            if (dim[0] == 0) {normal = sign*vec(1,0);}
            else if (dim[1] == 0) {normal = sign*vec(0,1);}
            else throw std::invalid_argument("vectors must have one component equal to each other");
        }

        surface(vec center, vec normal_vec, double l, int sign = 1) {
            if (normal_vec[0] == 0) {normal = sign*vec(0,1);}
            else if (normal_vec[1] == 0) {normal = sign*vec(1,0);}
            else throw std::invalid_argument("invalid normal vector");

            a = center - l/2*vec(center[1], -center[0]);
            b = center + l/2*vec(center[1], -center[0]);
            dim = (a-b).cwiseAbs();
        }


    public:
        vec a,b;
        vec dim;
        vec normal;
    };

}

#endif
