  #include "Triangle.h"
#include <iostream>

Triangle::Triangle(vec3 vert1, vec3 vert2, vec3 vert3, vec3 norm1, vec3 norm2, vec3 norm3) {
  _vertices[0] = vert1;
  _vertices[1] = vert2;
  _vertices[2] = vert3;
  _normals[0] = norm1;
  _normals[1] = norm2;
  _normals[2] = norm3;
  _normal = true;
  _type = tri;
}
Triangle::Triangle(vec3 vert1, vec3 vert2, vec3 vert3) {
  _vertices[0] = vert1;
  _vertices[1] = vert2;
  _vertices[2] = vert3;
  _normal = false;
  _type = tri;
}

void Triangle::calculateTransform(void) {
  if (!_transformed) {
    for (int i = 0; i < 3; ++i) {
      glm::vec3 point = _vertices[i];
      glm::vec4 homo_vec = glm::vec4(point[0], point[1], point[2], 1);
      homo_vec = homo_vec * _transform;
      _t_vertices[i] = glm::vec3(homo_vec[0] / homo_vec[3], homo_vec[1] / homo_vec[3], homo_vec[2] / homo_vec[3]);
    }
    if (_normal) {
      glm::mat4 inverse_transpose = glm::transpose(glm::inverse(_transform));
      for (int i = 0; i < 3; ++i) {
        glm::vec3 normal = _normals[i];
        glm::vec4 homo_norm = glm::vec4(normal[0], normal[1], normal[2], 1);
        homo_norm = homo_norm * inverse_transpose;
        _t_normals[i] = glm::vec3(homo_norm[0] / homo_norm[3], homo_norm[1] / homo_norm[3], homo_norm[2] / homo_norm[3]);
      }
    }
    _transformed = true;
  }
}

std::pair<bool,vec3> Triangle::intersect(vec3 origin, vec3 direction) {
  calculateTransform();
  vec3 p0 = _t_vertices[0], p1 = _t_vertices[1], p2 = _t_vertices[2];
  vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
  GLfloat t = glm::dot((p0 - origin), normal)/glm::dot(direction, normal);
  vec3 ray = origin + direction * t;
  bool hit = glm::dot(glm::cross(p1 - p0, ray - p0), normal) >= 0 &&
	     glm::dot(glm::cross(p2 - p1, ray - p1), normal) >= 0 &&
	     glm::dot(glm::cross(p0 - p2, ray - p2), normal) >= 0;
  return std::make_pair(hit, ray);
}
