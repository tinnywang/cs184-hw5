#ifndef OBJECT_H
#define OBJECT_H
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <utility>

typedef glm::vec3 vec3; 
typedef glm::vec4 vec4;

enum shape {sphere, tri} ;

class Object
{
public:
  Object() {};
	virtual ~Object() {};
  virtual std::pair<bool,vec3> intersect(vec3 origin, vec3 direction) {};

  shape _type ;
  vec4 _ambient ;
  vec4 _diffuse ;
  vec4 _specular ;
  vec4 _emission ;
  GLfloat _shininess ;
  glm::mat4 _transform ;
  bool _transformed ;
};

#endif
