#include "Sphere.h"

Sphere::Sphere(GLfloat pos1, GLfloat pos2, GLfloat pos3, GLfloat rad) {
  position[0] = pos1;
  position[1] = pos2;
  position[2] = pos3;
  radius = rad;
  type = sphere;
}

std::pair<bool,vec3> Sphere::intersect(vec3 origin, vec3 direction) {
  return std::make_pair(false, vec3(1,2,3));
}
