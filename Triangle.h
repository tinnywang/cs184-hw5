#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include <glm/glm.hpp>


typedef glm::vec3 vec3; 

class Triangle : public Object {
public:
	Triangle(vec3 vert1, vec3 vert2, vec3 vert3, vec3 norm1, vec3 norm2, vec3 norm3) ;
	Triangle(vec3 vert1, vec3 vert2, vec3 vert3) ;
	
	virtual ~Triangle() {};

  virtual std::pair<bool,vec3> intersect(vec3 ray);
  
private:
  vec3 vertices[3];
  vec3 normals[3];
  bool normal;
};

#endif
