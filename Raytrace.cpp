#include <limits>
#include <iostream>

#include <utility>
#include <FreeImage.h>
#include "variables.h"
#include "Raytrace.h"


void Raytrace::raytrace (vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, FIBITMAP* bitmap) {
  //std::cout << objects[0]->transform[2][1] << "\n";
  for (float i = 0; i < height; i++) {
    for (float j = 0; j < width; j++) {
      glm::vec3 ray_direction = calculateRay(eye, center, up, fovx, fovy, width, height, i+.5, j+.5);
      for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        std::pair<bool, vec3> result = (*it)->intersect(eye, ray_direction);
        if(result.first) {
          RGBQUAD color;
          color.rgbRed = 255 * (*it)->diffuse[0];
          color.rgbGreen = 255 * (*it)->diffuse[1];
          color.rgbBlue = 255 * (*it)->diffuse[2];
          FreeImage_SetPixelColor(bitmap, j, height - i - 1, &color);
        }
      }
      // calculate intersection of ray and object in scene
      // set pixel color
    }
  }
}


// returns the direction of the ray
glm::vec3 Raytrace::calculateRay(vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, float i, float j) {
  vec3 w = glm::normalize(eye - center);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::cross(w, u);
  float a = glm::tan(glm::radians(fovx/2)) * ((j-(static_cast<float>(width)/2))/(static_cast<float>(width)/2));
  float b = glm::tan(glm::radians(fovy/2)) * (((static_cast<float>(height)/2)-i)/(static_cast<float>(height)/2));
  return glm::normalize(a*u + b*v - w);
}
