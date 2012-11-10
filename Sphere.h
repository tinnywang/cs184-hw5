#ifndef SPHERE_H
#define SPHERE_H
#include "Object.h"
#include <glm/glm.hpp>

typedef glm::vec3 vec3; 

class Sphere : public Object
{
public:
	Sphere(GLfloat pos1, GLfloat pos2, GLfloat pos3, GLfloat radius) ;
	virtual ~Sphere() {};
	virtual std::pair<bool,vec3> intersect(vec3 ray);
  
private:
  GLfloat position[3];
  GLfloat radius;
};

#endif