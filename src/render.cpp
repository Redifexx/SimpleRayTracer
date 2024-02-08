#include "Camera.h"
#include <vector>
#include <cmath>
#include <iostream>

std::vector<std::vector<glm::uvec3>>& renderOutput(int screenWidth_, int screenHeight_) 
{
    std::vector<Sphere*> sphereList;
    std::vector<std::vector<glm::uvec3>>* render = new std::vector<std::vector<glm::uvec3>>(screenWidth_, std::vector<glm::uvec3>(screenHeight_));
    PerspectiveCam* cam = new PerspectiveCam(screenWidth_, screenHeight_);

    //Materials
    Material* red = new Material();
    red->baseColor = glm::uvec3(255, 0, 0);

    Material* green = new Material();
    green->baseColor = glm::uvec3(0, 255, 0);

    Material* blue = new Material();
    blue->baseColor = glm::uvec3(0, 0, 255);

    //Spheres
    Sphere* redSphere = new Sphere();
    redSphere->position = glm::vec3(-1.0, 1.0, -20.0);
    //redSphere->baseColor = glm::uvec3(255, 0, 0);
    sphereList.push_back(redSphere);

    Sphere* greenSphere = new Sphere();
    greenSphere->position = glm::vec3(1.0, 0.5, -7.0);
    greenSphere->radius = 0.5f;
    //greenSphere->baseColor = glm::uvec3(0, 255, 0);
    sphereList.push_back(greenSphere);

    Sphere* blueSphere = new Sphere();
    blueSphere->position = glm::vec3(1.0, -0.7, -15.0);
    blueSphere->radius = 1.0f;
    //blueSphere->baseColor = glm::uvec3(0, 0, 255);
    sphereList.push_back(blueSphere);

    //Light

    DirectionalLight* light = new DirectionalLight();
    light->strength = 2.0f;

    for (int i = 0; i < screenWidth_; i++)
    {
        for (int j = 0; j < screenHeight_; j++)
        {
            (*render)[i][j] = glm::uvec3(50, 50, 100);
            Ray* curRay = cam->getRay(i, j);
            for (int obj = 0; obj < sphereList.size(); obj++)
            {
                //std::cout << "TEST" << std::endl;
                glm::vec3 intersectionPoint;
                if (curRay->raySphereIntersection(sphereList[obj], intersectionPoint))
                {
                    (*render)[i][j] = green->shaderPixel(sphereList[obj]->surfaceNormal(intersectionPoint), light);
                }
            }
        }
    }
    return *render;
};
