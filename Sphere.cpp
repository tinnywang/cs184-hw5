#include "Sphere.h"
#include <iostream>

Sphere::Sphere(GLfloat pos1, GLfloat pos2, GLfloat pos3, GLfloat rad) {
    _position[0] = pos1;
    _position[1] = pos2;
    _position[2] = pos3;
    _radius = rad;
    _type = sphere;
}

std::pair<bool,vec3> Sphere::intersect(vec3 origin, vec3 direction) {
    glm::vec4 homo_origin = glm::vec4(origin[0], origin[1], origin[2], 1);
    glm::vec4 homo_direction = glm::vec4(direction[0], direction[1], direction[2], 0);
    homo_origin = homo_origin * glm::inverse(transform);
    homo_direction = homo_direction * glm::inverse(transform);
    origin = glm::vec3(homo_origin[0]/homo_origin[3], homo_origin[1]/homo_origin[3], homo_origin[2]/homo_origin[3]);
    direction = glm::vec3(homo_direction[0], homo_direction[1], homo_direction[2]);

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
    glm::vec3 intersect = origin + direction * t;
    glm::vec4 homo_inter = glm::vec4(intersect[0], intersect[1], intersect[2], 1);
    homo_inter = homo_inter * transform;
    intersect = glm::vec3(homo_inter[0]/homo_inter[3], homo_inter[1]/homo_inter[3], homo_inter[2]/homo_inter[3]);
    return std::make_pair(true, intersect);
}

vec3 Sphere::getNormal(vec3 intersect) {    
    glm::vec3 center = glm::vec3(_position[0], _position[1], _position[2]);
    glm::vec4 homo_inter = glm::vec4(intersect[0], intersect[1], intersect[2], 1);
    homo_inter = homo_inter * glm::inverse(transform);
    intersect = glm::vec3(homo_inter[0]/homo_inter[3], homo_inter[1]/homo_inter[3], homo_inter[2]/homo_inter[3]);

    vec3 normal = glm::normalize(intersect - center);
    vec4 homo_normal = vec4(normal.x, normal.y, normal.z, 0) * glm::transpose(glm::inverse(transform));
    return glm::normalize(vec3(homo_normal.x, homo_normal.y, homo_normal.z));
}

std::pair<vec3, vec3> Sphere::getBoundingBoxDimensions() {
  vec3 min = glm::vec3(_position[0] - _radius, _position[1] - _radius, _position[2] - _radius);
  vec3 max = glm::vec3(_position[0] + _radius, _position[1] + _radius, _position[2] + _radius);
  return std::make_pair(min, max);
}
  
