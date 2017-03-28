#ifndef GUARD_vec_h
#define GUARD_vec_h

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <stdexcept>
#include "h5cpp.h"
#include <vector>
#include <string>

namespace qbox {

    using vec = Eigen::Vector2d;
    using ivec = Eigen::Vector2i;
    using quat = Eigen::Quaterniond;
    using Array = Eigen::ArrayXd;

    enum class direction {
        x_bottom, x_top,
        y_bottom, y_top,
        //z_bottom, z_top,
    };

    template<class T, h5cpp::dtype M>
    void write_vec(h5cpp::h5group &group, const Eigen::Matrix<T,Eigen::Dynamic,1> &p, std::string name) {
        auto dspace = h5cpp::dspace(std::vector<hsize_t>{static_cast<unsigned long int>(p.size())});
        auto attr = group.create_attribute(name, M, dspace);
        attr.write(p.data());
    }


    template<class T, h5cpp::dtype M>
    struct surface_template {
        surface_template() = default;
        surface_template(Eigen::Matrix<T,2,1> a, Eigen::Matrix<T,2,1> b, int sign = 1): a(a), b(b), sign(sign) {
            dim = (a-b).cwiseAbs();

            if (dim[0] == 0) {normal = sign*Eigen::Matrix<T,2,1>(1,0);}
            else if (dim[1] == 0) {normal = sign*Eigen::Matrix<T,2,1>(0,1);}
            else throw std::invalid_argument("vectors must have one component equal to each other");
            tangent = (b-a)/(b-a).norm();
        }

        //surface_template(Eigen::Matrix<T,2,1> center, Eigen::Matrix<T,2,1> normal_vec, T l, int sign = 1): sign(sign) {
            //if (normal_vec[0] == 0) {normal = sign*Eigen::Matrix<T,2,1>(0,1);}
            //else if (normal_vec[1] == 0) {normal = sign*Eigen::Matrix<T,2,1>(1,0);}
            //else throw std::invalid_argument("invalid normal vector");

            //a = center - l/2*Eigen::Matrix<T,2,1>(center[1], -center[0]);
            //b = center + l/2*Eigen::Matrix<T,2,1>(center[1], -center[0]);
            //dim = (a-b).cwiseAbs();
        //}

        void write(h5cpp::h5group &group) const {
            write_vec<T,M>(group, a, "p1");
            write_vec<T,M>(group, b, "p2");
        }

    public:
        Eigen::Matrix<T,2,1> a,b;       ///< vector coordinates of two corners
        int sign;                       ///< 
        Eigen::Matrix<T,2,1> dim;       ///< vector containing the dimensions
        Eigen::Matrix<T,2,1> normal;    ///< unit vector normal to the surface
        Eigen::Matrix<T,2,1> tangent;   ///< unit vector that points from a to b
    };

    template<class T, h5cpp::dtype M>
    struct volume_template {
        volume_template() = default;
        volume_template(Eigen::Matrix<T,2,1> a, Eigen::Matrix<T,2,1> b): a(a), b(b) {
            dim = (a-b).cwiseAbs();
        }

        volume_template(Eigen::Matrix<T,2,1> center, T lx, T ly): dim(Eigen::Matrix<T,2,1>(lx,ly)) {
            a = center - Eigen::Matrix<T,2,1>(lx,ly)/2;
            b = center + Eigen::Matrix<T,2,1>(lx,ly)/2;
        }

        volume_template(Eigen::Matrix<T,2,1> center, T l): volume_template(center,l,l) {};

        void write(h5cpp::h5group &group) const {
            write_vec<T,M>(group, a, "p1");
            write_vec<T,M>(group, b, "p2");
        }


        surface_template<T,M> get_surface(direction dir) const {
            switch(dir) {
                case direction::y_bottom:
                    return surface_template<T,M>(a, vec(b[0], a[1])); break;
                case direction::y_top:
                    return surface_template<T,M>(b, vec(a[0], b[1])); break;
                case direction::x_bottom:
                    return surface_template<T,M>(vec(a[0], b[1]), a); break;
                case direction::x_top:
                    return surface_template<T,M>(vec(b[0], a[1]), b); break;
            }
        }

    public:
        Eigen::Matrix<T,2,1> a,b;
        Eigen::Matrix<T,2,1> dim;
    };

    using surface = surface_template<double, h5cpp::dtype::Double>;
    using isurface = surface_template<int, h5cpp::dtype::Int>;
    using volume = volume_template<double, h5cpp::dtype::Double>;
    using ivolume = volume_template<int, h5cpp::dtype::Int>;

}

#endif
