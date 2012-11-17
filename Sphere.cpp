#include "Sphere.h"

Sphere::Sphere(GLfloat pos1, GLfloat pos2, GLfloat pos3, GLfloat rad) {
  _position[0] = pos1;
  _position[1] = pos2;
  _position[2] = pos3;
  _radius = rad;
  _type = sphere;
}

std::pair<bool,vec3> Sphere::intersect(vec3 origin, vec3 direction) {
  vec3 center(_position[0], _position[1], _position[2]);
  GLfloat a = glm::dot(direction, direction);
  GLfloat b = 2 * glm::dot(direction, origin - center);
  GLfloat c = glm::dot(origin - center, origin - center) - (_radius * _radius);
  
  GLfloat discriminant = b * b - 4 * a * c;
  GLfloat t;
  if (discriminant < 0) {
    return std::make_pair(false, center);
  }
  GLfloat root1 = (-b + glm::sqrt(discriminant))/(2 * a);
  GLfloat root2 = (-b - glm::sqrt(discriminant))/(2 * a);
  if (root1 > 0 && root2 > 0) {
    t = glm::min(root1, root2);
  } else if (root1 == root2) {
    t = root1;
  } else if (root1 > 0 && root2 < 0) {
    t = root1;
  } else if (root1 < 0 && root2 > 0) {
    t = root2;
  } else {
    return std::make_pair(false, center);
  }
  return std::make_pair(true, origin + direction * t);
}
