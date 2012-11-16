#ifndef OBJECT_H
#define OBJECT_H
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <utility>

typedef glm::vec3 vec3; 

enum shape {sphere, tri} ;

class Object
{
public:
  Object() {};
	virtual ~Object() {};
  virtual std::pair<bool,vec3> intersect(vec3 origin, vec3 direction) {};
  
  shape _type ;
  GLfloat _ambient[4] ;
  GLfloat _diffuse[4] ;
  GLfloat _specular[4] ;
  GLfloat _emission[4] ;
  GLfloat _shininess ;
  glm::mat4 _transform ;
};

#endif
