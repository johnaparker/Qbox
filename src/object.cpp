#include "object.h"
#include <eigen3/Eigen/Geometry>
#include "math.h"

using namespace std;

namespace qbox {

    object::object(geometry& geometryType, material& materialType, vec center, vec orientation): 
                  geometryType(geometryType.clone()), materialType(materialType.clone()), center(center), orientation(orientation) {}

    bool object::inside(const vec& v) const {
        double theta = atan2(orientation(1), orientation(0)) - M_PI/2;
        auto R = Eigen::Rotation2Dd(theta);
        vec rot_v = R.toRotationMatrix()*(v-center);
        return geometryType->inside(rot_v); 
    }

    void object::move(const vec& dr) {
        center += dr;
    }

    void object::rotate(quat rot) {
        //orientation = rot._transformVector(orientation);
    }

    void object::write() {
    }

}
