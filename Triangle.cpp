#include "Triangle.h"

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

std::pair<bool,vec3> Triangle::intersect(vec3 ray) {
  return std::make_pair(true, vec3(1,2,3));
}
