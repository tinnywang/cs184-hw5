#include "Sphere.h"

Sphere::Sphere(GLfloat pos1, GLfloat pos2, GLfloat pos3, GLfloat rad) {
  _position[0] = pos1;
  _position[1] = pos2;
  _position[2] = pos3;
  _radius = rad;
  _type = sphere;
}

std::pair<bool,vec3> Sphere::intersect(vec3 origin, vec3 direction) {
  return std::make_pair(false, vec3(1,2,3));
}
