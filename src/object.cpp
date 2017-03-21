#include "object.h"

using namespace std;

namespace qbox {

    object::object(geometry& geometryType, material& materialType, vec center, vec orientation): 
                  geometryType(geometryType.clone()), materialType(materialType.clone()), center(center), orientation(orientation) {}

    bool object::inside(const vec& v) const {
       return geometryType->inside(v - center); 
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
