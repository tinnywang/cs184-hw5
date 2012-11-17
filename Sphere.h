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
	virtual std::pair<bool,vec3> intersect(vec3 origin, vec3 direction);
  
private:
  GLfloat _position[3];
  GLfloat _radius;
  GLfloat _t_position[3];
  GLfloat _t_radius;
};

#endif
