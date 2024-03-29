#include <limits>
#include <iostream>

#include <utility>
#include <FreeImage.h>
#include "variables.h"
#include "Raytrace.h"

namespace {
  void getPrunnedObjs(BoundingBox* node, const vec3& source, const vec3& direction, std::vector<std::pair<Object*, vec3> > * results) {
    std::pair<bool, glm::vec3> bb_result = node->intersect(source,direction);
    if (bb_result.first) {
      if (node->_obj != NULL) {
        results->push_back(std::make_pair(node->_obj, bb_result.second));        
      }
      if (node->_left_box != NULL) {
        if (node->_left_box->_obj != NULL) {
          results->push_back(std::make_pair(node->_left_box->_obj, bb_result.second));
        } else {          
          getPrunnedObjs(node->_left_box, source, direction, results);
        }
      }
      if (node->_right_box != NULL) {        
        if (node->_right_box->_obj != NULL) {          
          results->push_back(std::make_pair(node->_right_box->_obj, bb_result.second));
        } else {
          getPrunnedObjs(node->_right_box, source, direction, results);
        }
      }
    }    
  }
  
  std::pair<Object*, vec3> calculateIntersection(const vec3& eye, const vec3& ray_direction) {
    float min_distance = std::numeric_limits<float>::max();
    Object* i_obj = NULL;
    glm::vec3 intersection;
    std::vector<std::pair<Object*, vec3> > prunned_objects;    
    getPrunnedObjs(root_box, eye, ray_direction, &prunned_objects);  
    for (std::vector<std::pair<Object*, vec3> >::iterator it = prunned_objects.begin(); it != prunned_objects.end(); ++it) {
        if (min_distance != std::numeric_limits<float>::max()) {  // another optimization
          glm::vec3 difference = it->second - eye;
          float dist = glm::dot(difference, difference);
          if (dist >= min_distance) {
            continue;
          }
        }
        std::pair<bool, glm::vec3> result = (it->first)->intersect(eye, ray_direction);
        if (result.first) {
            glm::vec3 difference = result.second - eye;
            float dist = glm::dot(difference, difference);
            if (dist < min_distance) {
                min_distance = dist;                
                i_obj = it->first;
                intersection = result.second;
            }
        }
    }
    return std::make_pair(i_obj, intersection);
  }
}
void Raytrace::raytrace (const vec3& eye, const vec3& center, const vec3& up, float fovx, float fovy, int width, int height, FIBITMAP* bitmap, int recurse) {
    int count = 0;
    for (float i = 0; i < height; i++) {
        for (float j = 0; j < width; j++) {
            glm::vec3 ray_direction = calculateRay(eye, center, up, fovx, fovy, width, height, i+.5, j+.5);
            std::pair<Object*, vec3> i_result = calculateIntersection(eye, ray_direction);
            Object* i_obj = i_result.first;            
            glm::vec3 intersection = i_result.second; 
            if (i_obj != NULL) {
                vec4 phongColor = calculateColor(i_obj, intersection, eye, recurse);
                RGBQUAD color;
                color.rgbRed = 255 * phongColor.x;
                color.rgbGreen = 255 * phongColor.y;
                color.rgbBlue = 255 * phongColor.z;
                FreeImage_SetPixelColor(bitmap, j, height - i - 1, &color);
            }
        }
        std::cout << ++count << " rows out of " << height << " rows calculated.\n"; 
    }
}


// returns the direction of the ray
glm::vec3 Raytrace::calculateRay(const vec3& eye, const vec3& center, const vec3& up, float fovx, float fovy, int width, int height, float i, float j) {
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w, u);
    float a = glm::tan(glm::radians(fovx/2)) * ((j-(static_cast<float>(width)/2))/(static_cast<float>(width)/2));
    float b = glm::tan(glm::radians(fovy/2)) * (((static_cast<float>(height)/2)-i)/(static_cast<float>(height)/2));
    return glm::normalize(a*u + b*v - w);
}

glm::vec4 phongIllumination(const vec3& normal, const vec3& direction, const vec3& halfAngle, const vec4& lightcolor, float distance, const vec4& obj_diffuse, const vec4& obj_specular, GLfloat obj_shininess) {
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

glm::vec4 Raytrace::calculateColor(Object * obj, const vec3& intersection, const vec3& eye, int recurse) {
    vec4 finalcolor = vec4(0, 0, 0, 0);
    vec3 eyedir = glm::normalize(eye-intersection);
    vec3 normal = obj->getNormal(intersection);
    vec3 direction, halfAngle;
    float distance;
    float increment = .001;

    for (int i = 0; i < lightposn.size(); i++) {
        // shadow
        bool shadow = false;
        vec4 lightpos = lightposn[i];
        if (lightpos.w == 0) {
          direction = glm::normalize(glm::vec3(lightpos[0],lightpos[1],lightpos[2]));
          vec3 temp_inter = intersection + increment * direction;
          std::vector<std::pair<Object*, vec3> > prunned_objects;
          getPrunnedObjs(root_box, temp_inter, direction, &prunned_objects);
          for (std::vector<std::pair<Object*, vec3> >::iterator it = prunned_objects.begin(); it != prunned_objects.end(); ++it) {
            std::pair<bool, glm::vec3> result = (it->first)->intersect(temp_inter, direction);
            if (result.first) {              
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
          std::vector<std::pair<Object*, vec3> > prunned_objects;
          getPrunnedObjs(root_box, temp_inter, direction, &prunned_objects);
          
          for (std::vector<std::pair<Object*, vec3> >::iterator it = prunned_objects.begin(); it != prunned_objects.end(); ++it) {
            std::pair<bool, glm::vec3> result = (it->first)->intersect(temp_inter, direction);
            if (result.first) {
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
          distance = glm::sqrt(true_dist);
        }
        vec4 color = lightcolor[i];
        finalcolor += phongIllumination(normal, direction, halfAngle, color, distance, obj->_diffuse, obj->_specular, obj->_shininess);
    }
    finalcolor += obj->_ambient + obj->_emission;
    
    if (recurse > 0) {  // This is for reflection
      float times = 2 * glm::dot(eyedir, normal);
      vec3 reflection_direction = glm::normalize(-eyedir+(times * normal));
      vec3 temp_start = intersection + increment * reflection_direction;
      std::pair<Object*, vec3> i_result = calculateIntersection(temp_start, reflection_direction);
      Object* i_obj = i_result.first;
      glm::vec3 intersec = i_result.second;
      if (i_obj != NULL) {
        finalcolor += obj->_specular * calculateColor(i_obj, intersec, intersection, recurse - 1); 
      }
    }
    return glm::vec4(std::min(finalcolor[0],static_cast<float>(1)), std::min(finalcolor[1],static_cast<float>(1)),
                     std::min(finalcolor[2],static_cast<float>(1)), std::min(finalcolor[3],static_cast<float>(1)));
}

