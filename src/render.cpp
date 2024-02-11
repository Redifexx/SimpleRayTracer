#include "Camera.h"
#include "TGA.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>


std::vector<std::vector<glm::uvec3>>& renderOutput(int screenWidth_, int screenHeight_) 
{
    std::vector<Sphere*> sphereList;
    std::vector<std::vector<glm::uvec3>>* render = new std::vector<std::vector<glm::uvec3>>(screenWidth_, std::vector<glm::uvec3>(screenHeight_));
    Camera* cam = new Camera(screenWidth_, screenHeight_);

    //Materials
    Material* red = new Material();
    red->diffuseColor = rgbToFloat(glm::uvec3(255, 0, 0));
    red->ambientColor = rgbToFloat(glm::uvec3(255, 0, 0));

    Material* green = new Material();
    green->diffuseColor = rgbToFloat(glm::uvec3(0, 255, 0));
    green->ambientColor = rgbToFloat(glm::uvec3(0, 255, 0));

    Material* blue = new Material();
    blue->diffuseColor = rgbToFloat(glm::uvec3(0, 0, 255));
    blue->ambientColor = rgbToFloat(glm::uvec3(0, 0, 255));

    Material* white = new Material();

    //Spheres
    Sphere* redSphere = new Sphere(red);
    redSphere->position = glm::vec3(-3.0, 3.0, -15.0);
    redSphere->radius = 1.0f;
    //redSphere->baseColor = glm::uvec3(255, 0, 0);
    sphereList.push_back(redSphere);

    Sphere* greenSphere = new Sphere(green);
    greenSphere->position = glm::vec3(3.0, 3.0, -15.0);
    greenSphere->radius = 1.0f;
    //greenSphere->baseColor = glm::uvec3(0, 255, 0);
    sphereList.push_back(greenSphere);

    Sphere* blueSphere = new Sphere(blue);
    blueSphere->position = glm::vec3(-3.0, -3.0, -15.0);
    blueSphere->radius = 1.0f;
    //blueSphere->baseColor = glm::uvec3(0, 0, 255);
    sphereList.push_back(blueSphere);

    Sphere* whiteSphere = new Sphere(white);
    whiteSphere->position = glm::vec3(3.0, -3.0, -15.0);
    whiteSphere->radius = 1.0f;
    //blueSphere->baseColor = glm::uvec3(0, 0, 255);
    sphereList.push_back(whiteSphere);

    //Light
    DirectionalLight* light = new DirectionalLight();
    light->strength = 1.0f;
    light->lightRotate(glm::vec3(glm::radians(130.0f), glm::radians(0.0f), glm::radians(0.0f)));

    cam->camRotate(180.0f, 0.0f, -90.0f);
    cam->isPerspective = false;
    //cam->orthoScale = 10.0f;

    //Output TGA
    ImageFile* imageFile = new ImageFile(screenWidth_, screenHeight_);

    for (int i = 0; i < screenWidth_; i++)
    {
        for (int j = 0; j < screenHeight_; j++)
        {
            (*render)[i][j] = glm::uvec3(20, 20, 50);
            Ray* curRay = cam->getRay(i, j);
            imageFile->addPixel(i, j, 20, 20, 50);
            for (int obj = 0; obj < sphereList.size(); obj++)
            {
                glm::vec3 intersectionPoint;
                if (curRay->raySphereIntersection(sphereList[obj], intersectionPoint, cam->viewMatrix))
                {
                    glm::uvec3 result = sphereList[obj]->material->shaderPixel(sphereList[obj]->surfaceNormal(intersectionPoint), light, curRay->direction);
                    (*render)[i][j] = result;
                    imageFile->addPixel(i, j, result.x, result.y, result.z);
                }
            }
        }
    }
    imageFile->fileWrite();
    return *render;
};
