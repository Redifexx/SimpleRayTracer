#include "Camera.h"
#include "TGA.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>


std::vector<std::vector<glm::uvec3>>& renderOutput(int screenWidth_, int screenHeight_) 
{
    std::vector<Sphere*> sphereList;
    std::vector<Triangle*> triangleList;
    std::vector<Light*> lightList;
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

    Material* magenta = new Material();
    magenta->diffuseColor = rgbToFloat(glm::uvec3(255, 0, 255));
    magenta->ambientColor = rgbToFloat(glm::uvec3(255, 0, 255));
    magenta->diffuseIntensity = 0.2f;
    magenta->specularIntensity = 0.8f;

    Material* white = new Material();
    //white->diffuseIntensity = 0.2f;
    //white->specularIntensity = 0.8f;


    //Triangle Points
    glm::vec3 aVertex = glm::vec3(-2.0f, 0.0f, -2.0f);
    glm::vec3 bVertex = glm::vec3(2.0f, 0.0f, -2.0f);
    glm::vec3 cVertex = glm::vec3(0.0f, 2.0f, -2.0f);

    //Plane Points
    float scale = 1.0f; 
    glm::vec3 oneVertex = (glm::vec3(-5.0f, 0.0f, -5.0f) *  scale);
    glm::vec3 twoVertex = (glm::vec3(-5.0f, 0.0f, 5.0f) * scale);
    glm::vec3 threeVertex = (glm::vec3(5.0f, 0.0f, 5.0f) * scale);
    glm::vec3 fourVertex = (glm::vec3(5.0f, 0.0f, -5.0f) * scale);

    Sphere* redSphere;
    Sphere* greenSphere;
    Sphere* blueSphere;
    Sphere* whiteSphere;
    Sphere* newSphere;

    //Spheres for Triangle1
    redSphere = new Sphere(red);
    redSphere->position = aVertex;
    redSphere->radius = 0.3f;
    sphereList.push_back(redSphere);

    greenSphere = new Sphere(green);
    greenSphere->position = bVertex;
    greenSphere->radius = 0.3f;
    sphereList.push_back(greenSphere);

    blueSphere = new Sphere(blue);
    blueSphere->position = cVertex;
    blueSphere->radius = 0.3f;
    sphereList.push_back(blueSphere);

    whiteSphere = new Sphere(white);
    whiteSphere->position = glm::vec3(0.0, 4.5, -5.0);
    whiteSphere->radius = 1.0f;
    sphereList.push_back(whiteSphere);

    whiteSphere = new Sphere(white);
    whiteSphere->position = glm::vec3(0.0, 7.0, -7.0);
    whiteSphere->radius = 0.6f;
    sphereList.push_back(whiteSphere);

    newSphere = new Sphere(magenta);
    newSphere->position = glm::vec3(0.0, 0.0, 0.0);
    newSphere->radius = 1.0f;
    sphereList.push_back(newSphere);

    //
    //Spheres for Triangle2
    redSphere = new Sphere(red);
    redSphere->position = oneVertex;
    redSphere->radius = 0.3f;
    //redSphere->baseColor = glm::uvec3(255, 0, 0);
    sphereList.push_back(redSphere);

    greenSphere = new Sphere(green);
    greenSphere->position = threeVertex;
    greenSphere->radius = 0.3f;
    //greenSphere->baseColor = glm::uvec3(0, 255, 0);
    sphereList.push_back(greenSphere);

    blueSphere = new Sphere(blue);
    blueSphere->position = twoVertex;
    blueSphere->radius = 0.3f;
    //blueSphere->baseColor = glm::uvec3(0, 0, 255);
    sphereList.push_back(blueSphere);

    blueSphere = new Sphere(white);
    blueSphere->position = fourVertex;
    blueSphere->radius = 0.3f;
    //blueSphere->baseColor = glm::uvec3(0, 0, 255);
    sphereList.push_back(blueSphere);

    //Light
    Light* light = new Light();
    light->strength = 1.0f;
    light->lightRotate(glm::vec3(glm::radians(200.0f), glm::radians(45.0f), glm::radians(0.0f)));
    lightList.push_back(light);

    
    light = new Light();
    light->strength = 1.0f;
    light->position = glm::vec3(2.0f, 2.0f, -2.0f);
    light->baseColor = glm::vec3(1.0f, 0.0, 0.0f);
    light->isPoint = true;
    lightList.push_back(light);
    
    light = new Light();
    light->strength = 3.0f;
    light->position = glm::vec3(-3.0f, 3.0f, 2.0f);
    light->baseColor = glm::vec3(0.0f, 0.0, 1.0f);
    light->isPoint = true;
    lightList.push_back(light);

    //Triangle
    Triangle* triangle = new Triangle(aVertex, bVertex, cVertex, white);
    //triangleList.push_back(triangle);

    Object* plane = new Object(white);
    triangle = new Triangle(threeVertex, twoVertex, oneVertex, plane->material);
    plane->triangles.push_back(triangle);
    triangleList.push_back(triangle);
    triangle = new Triangle(oneVertex, fourVertex, threeVertex, plane->material);
    plane->triangles.push_back(triangle);
    triangleList.push_back(triangle);

    std::cout << "SIZE " << triangleList.size() << std::endl;

    //Camera
    cam->camPos = glm::vec3(0.0f, 5.0f, 15.0f);
    float camAngle = 180.0f;
    cam->camRotate(180.0f, -15.0f, -90.0f);
    cam->isPerspective = true;
    cam->perspectiveDistance = 10.0f;
    //cam->orthoScale = 10.0f;
    
    for (int seq = 0; seq < 1; seq++)
    {
        //Output TGA
        ImageFile* imageFile = new ImageFile(screenWidth_, screenHeight_);
        float t1 = 0.0f;
        float t2 = cam->maxRayLength;
        for (int i = 0; i < screenWidth_; i++)
        {
            for (int j = 0; j < screenHeight_; j++)
            {
                float t0 = cam->maxRayLength; //Closest to screen
                (*render)[i][j] = glm::uvec3(20, 50, 50);
                Ray* curRay = cam->getRay(i, j);
                imageFile->addPixel(i, j, 20, 50, 50);

                for (int tri = 0; tri < triangleList.size(); tri++)
                {
                    glm::vec3 intersectionPoint;
                    if (curRay->rayTriangleIntersection(triangleList[tri], intersectionPoint, t0, t1, t2))
                    {
                        glm::vec3 result = triangleList[tri]->material->shaderPixel(-triangleList[tri]->surfaceNormal(), lightList, curRay->direction, intersectionPoint, triangleList, sphereList);
                        (*render)[i][j] = result;
                        imageFile->addPixel(i, j, result.x, result.y, result.z);
                    }
                }

                for (int obj = 0; obj < sphereList.size(); obj++)
                {
                    glm::vec3 intersectionPoint;
                    if (curRay->raySphereIntersection(sphereList[obj], intersectionPoint, t0, t1, t2))
                    {
                        glm::uvec3 result = sphereList[obj]->material->shaderPixel(sphereList[obj]->surfaceNormal(intersectionPoint), lightList, curRay->direction, intersectionPoint, triangleList, sphereList);
                        (*render)[i][j] = result;
                        imageFile->addPixel(i, j, result.x, result.y, result.z);
                    }
                }
            }
        }
        imageFile->fileName = "../output/" + std::to_string(seq) + ".tga";
        imageFile->fileWrite();
        std::cout << imageFile->fileName << ": FILE WRITTEN" << std::endl;
        //cam->camPos.x -= 3.0f;
        //aVertex.z += 1.0f;
        //cVertex.z -= 1.0f;
        //triangle->vertices[0] = aVertex;
        //triangle->vertices[1] = bVertex;
        //triangle->vertices[2] = cVertex;
        //redSphere->position = aVertex;
        //greenSphere->position = bVertex;
        //blueSphere->position = cVertex;
        //cam->camPos = glm::vec3(cam->camPos.x, 0.0f, -3.0f);
        //camAngle -= 22.5f;
        //cam->camRotate(camAngle, 0.0f, -90.0f);
    }
    return *render;
};
