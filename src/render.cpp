#include "Camera.h"
#include <vector>
#include <cmath>

std::vector<std::vector<glm::uvec3>>& renderOutput(int screenWidth_, int screenHeight_) 
{
    std::vector<std::vector<glm::uvec3>>* render = new std::vector<std::vector<glm::uvec3>>(screenWidth_, std::vector<glm::uvec3>(screenHeight_));
    PerspectiveCam* cam = new PerspectiveCam(screenWidth_, screenHeight_);
    Sphere* sphere = new Sphere();
    sphere->position = glm::vec3(0.0, 0.0, -30.0);
    for (int i = 0; i < screenWidth_; i++)
    {
        for (int j = 0; j < screenHeight_; j++)
        {
            Ray* curRay = cam->getRay(i, j);
            bool sphereIntersection = false;
            float a = (glm::dot(curRay->direction, curRay->direction));
            float b = (glm::dot((curRay->direction * 2.0f), (curRay->origin - sphere->position)));
            float c = (glm::dot((curRay->origin - sphere->position), (curRay->origin - sphere->position))) - std::pow(sphere->radius, 2);
            float discriminant = (std::pow(b, 2) - 4*a*c);
            if (discriminant > 0.0)
            {
                float t1 = ((-b) - std::sqrt(discriminant)) / 2*a;
                sphereIntersection = true;
            }
            if (sphereIntersection)
            {
                (*render)[i][j] = glm::uvec3(255, 50, 50);
            } else
            {
                (*render)[i][j] = glm::uvec3(20, 20, 20);
            }
        }
    }
    return *render;
};
