#include <limits>
#include <iostream>

#include <utility>
#include <FreeImage.h>
#include "variables.h"
#include "Raytrace.h"


void Raytrace::raytrace (vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, FIBITMAP* bitmap) {
  for (float i = 0; i < width; i++) {
    for (float j = 0; j < height; j++) {
      glm::vec3 ray = calculateRay(eye, center, up, fovx, fovy, width, height, i+.5, j+.5);
      for (std::vector<Object>::iterator it = objects.begin(); it != objects.end(); ++it) {
        std::pair<bool, vec3> result = it->intersect(ray);
        if(result.first) {
          RGBQUAD color;
          color.rgbRed = it->diffuse[0];
          color.rgbGreen = it->diffuse[1];
          color.rgbBlue = it->diffuse[2];
          FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
        break;
      }
      // calculate intersection of ray and object in scene
      // set pixel color
    }
  }
}

glm::vec3 Raytrace::calculateRay(vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, float i, float j) {
  vec3 w = glm::normalize(eye - center);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::cross(w, u);
  float a = glm::tan(fovx/2) * ((j-(width/2.0))/(width/2.0));
  float b = glm::tan(fovy/2) * (((height/2.0)-i)/(height/2.0));
  return eye + glm::normalize(a*u + b*v - w);
}
