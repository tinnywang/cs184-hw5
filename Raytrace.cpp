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
                vec4 phongColor = calculateColor(i_obj, intersection);
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

glm::vec4 phongIllumination(vec3 normal, vec3 direction, vec3 halfAngle, vec4 lightcolor, float distance) {
    float nDotL = glm::dot(normal, direction);
    if (nDotL < 0) {
        nDotL = 0;
    }
    vec4 diffuseTerm = glm::vec4(diffuse.x * nDotL, diffuse.y * nDotL, diffuse.z * nDotL, diffuse.w * nDotL);
    float nDotH = glm::dot(normal, halfAngle);
    if (nDotH < 0) {
        nDotH = 0;
    }
    float shine = glm::pow(nDotH, shininess);
    vec4 specularTerm = glm::vec4(specular.x * shine, specular.y * shine, specular.z * shine, specular.w * shine);
    if (distance == 0) {  // directional lights have no attenuation.
        return lightcolor * (diffuseTerm + specularTerm);
    } else {
        return (lightcolor / (attenuation.x + attenuation.y * distance + attenuation.z * distance * distance)) * (diffuseTerm + specularTerm);
    }
}

glm::vec4 Raytrace::calculateColor(Object * obj, const vec3& intersection) {
    vec4 finalcolor = vec4(0, 0, 0, 0);
    vec3 eyedir = glm::normalize(eye-intersection);
    vec3 normal = obj->getNormal(intersection);
    vec3 direction, halfAngle, difference;
    float distance;

    for (int i = 0; i < lightposn.size(); i++) {
        vec4 color = lightcolor[i];
        vec4 lightpos = lightposn[i];
        if (lightpos.w == 0) {
            vec3 temp = glm::vec3(lightpos.x, lightpos.y, lightpos.z);
            direction = glm::normalize(temp);
            halfAngle = glm::normalize(direction + eyedir);
            distance = 0;
        } else {
            vec3 position = glm::vec3(lightpos.x/lightpos.w, lightpos.y/lightpos.w, lightpos.z/lightpos.w);
            direction = glm::normalize(position - intersection);
            halfAngle = glm::normalize(direction + eyedir);
            difference = position - intersection;
            distance = glm::sqrt(glm::dot(difference, difference));
        }
        finalcolor += phongIllumination(normal, direction, halfAngle, color, distance);
    }
    finalcolor += obj->_ambient + obj->_emission;
    return finalcolor;
}

