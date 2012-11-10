#include "Triangle.h"
#include <iostream>

Triangle::Triangle(vec3 vert1, vec3 vert2, vec3 vert3, vec3 norm1, vec3 norm2, vec3 norm3) {
  vertices[0] = vert1;
  vertices[1] = vert2;
  vertices[2] = vert3;
  normals[0] = norm1;
  normals[1] = norm2;
  normals[2] = norm3;
  normal = true;
  type = tri;
}
Triangle::Triangle(vec3 vert1, vec3 vert2, vec3 vert3) {
  vertices[0] = vert1;
  vertices[1] = vert2;
  vertices[2] = vert3;
  normal = false;
  type = tri;
}

std::pair<bool,vec3> Triangle::intersect(vec3 origin, vec3 direction) {
  vec3 p0 = vertices[0], p1 = vertices[1], p2 = vertices[2];
  vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
  GLfloat t = -glm::dot((origin - p0), normal)/glm::dot(direction, normal);
  vec3 ray = origin + direction * t;
  bool hit = glm::dot(glm::cross(p1 - p0, ray - p0), normal) >= 0 &&
	     glm::dot(glm::cross(p2 - p1, ray - p1), normal) >= 0 &&
	     glm::dot(glm::cross(p0 - p2, ray - p2), normal) >= 0;
  return std::make_pair(hit, ray);
}
