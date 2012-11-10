#ifndef RAYTRACE_H
#define RAYTRACE_H
#include <glm/glm.hpp>

typedef glm::vec3 vec3; 

class FIBITMAP;

class Raytrace  
{
public:
	Raytrace() {};
	virtual ~Raytrace() {};
	void raytrace (vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, FIBITMAP* bitmap);
	glm::vec3 calculateRay(vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, float i, float j);
};

#endif
