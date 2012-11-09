#include <glm/glm.hpp>

typedef glm::vec3 vec3; 

class Raytrace  
{
public:
	Raytrace();
	virtual ~Raytrace();
	static void raytrace (vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height);
	static glm::vec3 calculateRay(vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, float i, float j);
};

