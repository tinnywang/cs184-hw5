#include "BoundingBox.h"

BoundingBox::BoundingBox(Object * obj) {
  _obj = obj;
  _type = boundingbox;
  std::pair<vec3, vec3> dimensions = obj->getBoundingBoxDimensions();
  _min = dimensions.first;
  _max = dimensions.second;
}

std::pair<bool, vec3> BoundingBox::intersect(vec3 origin, vec3 direction) {
    return _obj->intersect(origin, direction);
}

Object * BoundingBox::getObject() {
  return _obj;
}
