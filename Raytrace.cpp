#include <limits>
#include <iostream>

#include <utility>
#include <FreeImage.h>
#include "variables.h"
#include "Raytrace.h"

void Raytrace::raytrace (vec3& eye, vec3& center, vec3& up, float fovx, float fovy, int width, int height, FIBITMAP* bitmap, int recurse) {
    //std::cout << objects[0]->transform[2][1] << "\n";
    for (float i = 0; i < height; i++) {
        for (float j = 0; j < width; j++) {
            glm::vec3 ray_direction = calculateRay(eye, center, up, fovx, fovy, width, height, i+.5, j+.5);
            float min_distance = std::numeric_limits<float>::max();
            Object* i_obj;
            glm::vec3 intersection;   // idk if u need this
            for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                std::pair<bool, glm::vec3> result = (*it)->intersect(eye, ray_direction);
                if(result.first) {
                    glm::vec3 difference = result.second - eye;
                    float dist = glm::dot(difference, difference);
                    if (dist < min_distance) {
                        min_distance = dist;
                        i_obj = *it;
                        intersection = result.second;
                    }
                }
            }
            if (min_distance != std::numeric_limits<float>::max()) {
                vec4 phongColor = calculateColor(i_obj, intersection, recurse);
                RGBQUAD color;
                color.rgbBlue = 255 * phongColor.x;
                color.rgbGreen = 255 * phongColor.y;
                color.rgbRed = 255 * phongColor.z;
                FreeImage_SetPixelColor(bitmap, j, height - i - 1, &color);
            }
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

glm::vec4 phongIllumination(vec3 normal, vec3 direction, vec3 halfAngle, vec4 lightcolor, float distance, vec4 obj_diffuse, vec4 obj_specular, GLfloat obj_shininess) {
    float nDotL = glm::dot(normal, direction);
    if (nDotL < 0) {
        nDotL = 0;
    }
    vec4 diffuseTerm = glm::vec4(obj_diffuse.x * nDotL, obj_diffuse.y * nDotL, obj_diffuse.z * nDotL, obj_diffuse.w * nDotL);
    float nDotH = glm::dot(normal, halfAngle);
    if (nDotH < 0) {
        nDotH = 0;
    }
    float shine = glm::pow(nDotH, obj_shininess);
    vec4 specularTerm = glm::vec4(obj_specular.x * shine, obj_specular.y * shine, obj_specular.z * shine, obj_specular.w * shine);
    if (distance == 0) {  // directional lights have no attenuation.
        return lightcolor * (diffuseTerm + specularTerm);
    } else {
        return (lightcolor / (attenuation.x + attenuation.y * distance + attenuation.z * distance * distance)) * (diffuseTerm + specularTerm);
    }
}

glm::vec4 Raytrace::calculateColor(Object * obj, const vec3& intersection, int recurse) {
    vec4 finalcolor = vec4(0, 0, 0, 0);
    vec3 eyedir = glm::normalize(eye-intersection);
    vec3 normal = obj->getNormal(intersection);
    vec3 direction, halfAngle, difference;
    float distance;
    float increment = .001;

    for (int i = 0; i < lightposn.size(); i++) {
        // shadow
        bool shadow = false;
        vec4 lightpos = lightposn[i];
        if (lightpos.w == 0) {
          direction = glm::normalize(glm::vec3(lightpos[0],lightpos[1],lightpos[2]));
          vec3 temp_inter = intersection + increment * direction;
          for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
            std::pair<bool, glm::vec3> result = (*it)->intersect(temp_inter, direction);
            if (result.first && glm::dot(normal, direction) >= 0) {              
              shadow = true;
              break;
            }
          }
          if (shadow) {
            continue;
          }
          halfAngle = glm::normalize(direction + eyedir);
          distance = 0;
        } else {
          vec3 dir_vec = glm::vec3(lightpos[0] - intersection.x,
                                   lightpos[1] - intersection.y,
                                   lightpos[2] - intersection.z);
          direction = glm::normalize(dir_vec);
          vec3 temp_inter = intersection + increment * direction;
          float true_dist = glm::dot(dir_vec,dir_vec);
          for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
            std::pair<bool, glm::vec3> result = (*it)->intersect(temp_inter, direction);
            if (result.first && glm::dot(normal, direction) >= 0) {
              vec3 diff_vec = glm::vec3(result.second.x - temp_inter.x,
                                        result.second.y - temp_inter.y,
                                        result.second.z - temp_inter.z);
              float diff_dist = glm::dot(diff_vec, diff_vec);
              if (diff_dist < true_dist) {
                shadow = true;
                break;
              }
            }
          }
          if (shadow) {
            continue;
          }
          halfAngle = glm::normalize(direction + eyedir);
          distance = true_dist;
        }
        vec4 color = lightcolor[i];
        finalcolor += phongIllumination(normal, direction, halfAngle, color, distance, obj->_diffuse, obj->_specular, obj->_shininess);
    }
    finalcolor += obj->_ambient + obj->_emission;
    
    if (recurse != 0) {  // This is for reflection
      float times = 2 * glm::dot(-eyedir, normal);
      vec3 reflection_direction = glm::normalize(-eyedir-(times * normal));
      float min_distance = std::numeric_limits<float>::max();
      Object* i_obj;
      glm::vec3 intersec;   // idk if u need this
      vec3 temp_start = intersection + increment * reflection_direction;
      
      for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
          std::pair<bool, glm::vec3> result = (*it)->intersect(temp_start, reflection_direction);
          if(result.first) {
              glm::vec3 diff = result.second - temp_start;
              float dist = glm::dot(diff, diff);
              if (dist < min_distance) {
                  min_distance = dist;
                  i_obj = *it;
                  intersec = result.second;
              }
          }
      }
      if (min_distance != std::numeric_limits<float>::max()) {
        finalcolor += obj->_specular * calculateColor(i_obj, intersec, recurse - 1); 
      }
    }
    return glm::vec4(std::min(finalcolor[0],static_cast<float>(1)), std::min(finalcolor[1],static_cast<float>(1)),
                     std::min(finalcolor[2],static_cast<float>(1)), std::min(finalcolor[3],static_cast<float>(1)));
}

