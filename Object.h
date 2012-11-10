#ifndef OBJECT_H
#define OBJECT_H
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <utility>

typedef glm::vec3 vec3; 

enum shape {sphere, tri} ;

class Object
{
public:
  Object() {};
	virtual ~Object() {};
  virtual std::pair<bool,vec3> intersect(vec3 ray) {};
  
  shape type ;
  GLfloat ambient[4] ;
  GLfloat diffuse[4] ;
  GLfloat specular[4] ;
  GLfloat emission[4] ;
  GLfloat shininess ;
  glm::mat4 transform ;
};

#endif
