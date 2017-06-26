#include "geometry/geometry.h"

namespace qbox {

    std::optional<volume> geometry::get_bounding_box(double theta) const {
        std::optional<volume> box;
        if (bounding_box) {
            Eigen::Matrix2d R = Eigen::Rotation2Dd(theta).toRotationMatrix();
            R = R.array().abs();
            vec new_dim = R*bounding_box->dim;
            box = volume(-new_dim/2, new_dim/2);
        }
        return box;
    }

}
