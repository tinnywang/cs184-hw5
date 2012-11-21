#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

typedef glm::vec3 vec3;

class BoundingBox : public Object
{
public:
  BoundingBox(Object *object);
  BoundingBox(Object *obj1, Object *obj2);
  virtual ~BoundingBox();
  virtual std::pair<bool, vec3> intersect(const vec3& origin, const vec3& direction);
  virtual Object * getObject();

  Object * leftObj = NULL;
  Object * iightObj = NULL;
private:
  vec3 _min;
  vec3 _max;
};
  
#endif  
