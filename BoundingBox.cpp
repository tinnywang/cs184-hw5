#include "BoundingBox.h"
#include <iostream>

BoundingBox::BoundingBox(Object * obj) {
  _obj = obj;
  _type = boundingbox;
  std::pair<vec3, vec3> dimensions = obj->getBoundingBoxDimensions();
  _min = dimensions.first;
  _max = dimensions.second;
}

std::pair<bool, vec3> BoundingBox::intersect(vec3 origin, vec3 direction) {
    float t, inf = std::numeric_limits<float>::max();
    vec3 x = vec3(1, 0, 0), y = vec3(0, 1, 0), z = vec3(0, 0, 1), rtn = vec3(inf, inf, inf);
    vec3 intersection;
    // near face
    if (glm::dot(direction, z) != 0) {
      t = -glm::dot((origin - _max), z)/glm::dot(direction, z);
      intersection = origin + direction * t;
      if (_min.x < intersection.x && intersection.x < _max.x && _min.y < intersection.y && intersection.y < _max.y) {
	return _obj->intersect(origin, direction);
      }
    } 
    // far face
    if (glm::dot(direction, -z) != 0) {
      t = -glm::dot((origin - _min), -z)/glm::dot(direction, -z);
      intersection = origin + direction * t;
      if (_min.x < intersection.x && intersection.x < _max.x && _min.y < intersection.y && intersection.y < _max.y) {
        return _obj->intersect(origin, direction);
      }
    }
    // right face
    if (glm::dot(direction, x) != 0) {
      t = -glm::dot((origin - _max), x)/glm::dot(direction, x);
      intersection = origin + direction * t;
      if (_min.z < intersection.z && intersection.z < _max.z && _min.y < intersection.y && intersection.y < _max.y) {
        return _obj->intersect(origin, direction);
      }
    }
    // left face
    if (glm::dot(direction, -x) != 0) {
      t = -glm::dot((origin - _min), -x)/glm::dot(direction, -x);
      intersection = origin + direction * t;
      if (_min.z < intersection.z && intersection.z < _max.z && _min.y < intersection.y && intersection.y < _max.y) {
	return _obj->intersect(origin, direction);
      }
    }
    // top face
    if (glm::dot(direction, y) != 0) {
      t = -glm::dot((origin - _max), y)/glm::dot(direction, y);
      intersection = origin + direction * t;
      if (_min.z < intersection.z && intersection.z < _max.z && _min.x < intersection.x && intersection.x < _max.x) {
        return _obj->intersect(origin, direction);
      }
    }
    // bottom face
    if (glm::dot(direction, -y) != 0) {
      t = -glm::dot((origin - _min), -y)/glm::dot(direction, -y);
      intersection = origin + direction * t;
      if (_min.z < intersection.z && intersection.z < _max.z && _min.x < intersection.x && intersection.x < _max.x) {
	return _obj->intersect(origin, direction);
      }
    }
    return std::make_pair(false, origin);
}

Object * BoundingBox::getObject() {
  return _obj;
}
