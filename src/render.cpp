#include "Camera.h"
#include "TGA.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>


std::vector<std::vector<glm::uvec3>>& renderOutput(int screenWidth_, int screenHeight_, bool isOrtho) 
{
    float renderTime = 0.0f;
    std::vector<Sphere*> sphereList;
    std::vector<Triangle*> triangleList;
    std::vector<Light*> lightList;

    std::vector<std::vector<glm::uvec3>>* render = new std::vector<std::vector<glm::uvec3>>(screenWidth_, std::vector<glm::uvec3>(screenHeight_));
    Camera* cam = new Camera(screenWidth_, screenHeight_);

    //
    //Materials
    Material* white = new Material();

    Material* red = new Material();
    red->diffuseColor = rgbToFloat(glm::uvec3(255, 0, 0));
    red->ambientColor = rgbToFloat(glm::uvec3(255, 0, 0));
    red->diffuseIntensity = 0.3f;
    red->specularIntensity = 0.7f;

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

    Material* orange = new Material();
    orange->diffuseColor = rgbToFloat(glm::uvec3(255, 70, 0));
    orange->ambientColor = rgbToFloat(glm::uvec3(255, 70, 0));
    orange->diffuseIntensity = 0.5f;
    orange->specularIntensity = 0.5f;

    Material* teal = new Material();
    teal->diffuseColor = rgbToFloat(glm::uvec3(51, 204, 204));
    teal->ambientColor = rgbToFloat(glm::uvec3(51, 204, 204));
    teal->diffuseIntensity = 0.5f;
    teal->specularIntensity = 0.5f;

    Material* glossy = new Material();
    glossy->diffuseColor = rgbToFloat(glm::uvec3(255, 255, 255));
    glossy->ambientColor = rgbToFloat(glm::uvec3(255, 255, 255));
    glossy->diffuseIntensity = 0.0f;
    glossy->specularIntensity = 1.0f;
    glossy->metallicFactor = 1.0f;


    //
    //Pyramid Points
    glm::vec3 aVertex = glm::vec3(-1.0f, 2.0f, 1.0f);
    glm::vec3 bVertex = glm::vec3(1.0f, 2.0f, 1.0f);
    glm::vec3 cVertex = glm::vec3(1.0f, 2.0f, -1.0f);
    glm::vec3 dVertex = glm::vec3(-1.0f, 2.0f, -1.0f);
    glm::vec3 eVertex = glm::vec3(0.0f, 5.0f, 0.0f);

    //
    //Plane Points
    float scale = 10.0f; 
    glm::vec3 oneVertex = (glm::vec3(-5.0f, 0.0f, -5.0f) *  scale);
    glm::vec3 twoVertex = (glm::vec3(-5.0f, 0.0f, 5.0f) * scale);
    glm::vec3 threeVertex = (glm::vec3(5.0f, 0.0f, 5.0f) * scale);
    glm::vec3 fourVertex = (glm::vec3(5.0f, 0.0f, -5.0f) * scale);

    //Spheres
    Sphere* sphere;

    //Sphere - Glossy
    sphere = new Sphere(glossy);
    sphere->position = glm::vec3(-5.0, 2.0, 0.0);
    sphere->radius = 1.0f;
    sphereList.push_back(sphere);

    //Sphere - Diffuse
    sphere = new Sphere(red);
    sphere->position = glm::vec3(5.0, 2.0, 0.0);
    sphere->radius = 1.0f;
    sphereList.push_back(sphere);

    //
    //Lights

    //Directional - Straight Down
    Light* light = new Light();
    light->strength = 1.0f;
    light->lightRotate(glm::vec3(glm::radians(180.0f), glm::radians(70.0f), glm::radians(0.0f)));
    lightList.push_back(light);

    //Point1
    light = new Light();
    light->strength = 10.0f;
    light->position = glm::vec3(-2.5f, 4.0f, -5.0f);
    light->baseColor = glm::vec3(1.0f, 0.0f, 0.0f);
    light->isPoint = true;
    lightList.push_back(light);
    
    //Point 2
    light = new Light();
    light->strength = 10.0f;
    light->position = glm::vec3(2.5f, 4.0f, -5.0f);
    light->baseColor = glm::vec3(0.0f, 1.0, 0.0f);
    light->isPoint = true;
    lightList.push_back(light);

    //Point 3
    light = new Light();
    light->strength = 10.0f;
    light->position = glm::vec3(0.0f, 4.0f, 5.0f);
    light->baseColor = glm::vec3(0.0f, 0.0, 1.0f);
    light->isPoint = true;
    lightList.push_back(light);

    //
    //Triangle
    Triangle* triangle;

    //Plane Triangles
    Object* plane = new Object(white);

    triangle = new Triangle(threeVertex, twoVertex, oneVertex, plane->material);
    plane->triangles.push_back(triangle);
    triangleList.push_back(triangle);

    triangle = new Triangle(oneVertex, fourVertex, threeVertex, plane->material);
    plane->triangles.push_back(triangle);
    triangleList.push_back(triangle);

    
    //Tetrahedron Triangles
    triangle = new Triangle(bVertex, aVertex, eVertex, red);
    //triangleList.push_back(triangle);

    triangle = new Triangle(cVertex, bVertex, eVertex, blue);
    //triangleList.push_back(triangle);

    triangle = new Triangle(dVertex, cVertex, eVertex, green);
    triangleList.push_back(triangle);

    triangle = new Triangle(aVertex, dVertex, eVertex, white);
    //triangleList.push_back(triangle);

    //triangle = new Triangle(aVertex, bVertex, cVertex, orange);
    //triangleList.push_back(triangle);

    //triangle = new Triangle(aVertex, cVertex, dVertex, orange);
    //triangleList.push_back(triangle);
    
    //
    //Camera

    glm::vec3 camPosition = glm::vec3(10.0f, 3.0f, 10.0f);
    cam->camPos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);

    //(L-R)
    glm::vec3 camRotation = glm::vec3(135.0f, -5.0f, -85.0f);
    cam->camRotate(camRotation.x, camRotation.y, camRotation.z);

    cam->isPerspective = !isOrtho;
    cam->perspectiveDistance = 6.0f;
    
    //Camera Animation
    //int frames = 5;
    //float orbitRadius = 10.0f;
    //float orbitSpeed = glm::radians(360.0f) / frames;

    //
    //Additional Settings

    //Reflection Depth
    int lightBounces = 3;

    //Infinite Plane Specification
    float planeDepth = -1.0f; 

    //Seq is how many rendered frames
    for (int seq = 0; seq < 1; seq++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        //std::cout << "READY TO RENDER!" << std::endl;
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
                glm::vec3 intersectionPoint = glm::vec3(0.0f);
                //if (curRay->rayPlaneIntersection(planeDepth, intersectionPoint, t0, t1, t2))
                //{
                    //glm::vec3 result = white->shaderPixel(glm::vec3(0.0f, 1.0f, 0.0f), lightList, curRay->direction, lightBounces, intersectionPoint, triangleList, sphereList);
                    //(*render)[i][j] = result;
                    //imageFile->addPixel(i, j, result.x, result.y, result.z);
               // }
                
                //Triangle Loop
                
                for (int tri = 0; tri < triangleList.size(); tri++)
                {
                    intersectionPoint = glm::vec3(0.0f);
                    float alpha;
                    float beta;
                    float gamma;
                    if (curRay->rayTriangleIntersection(triangleList[tri], intersectionPoint, t0, t1, t2, alpha, beta, gamma))
                    {
                        glm::vec3 result = triangleList[tri]->material->shaderPixel(-triangleList[tri]->weightedNormal(alpha, beta, gamma), lightList, curRay, lightBounces, intersectionPoint, triangleList, sphereList);
                        (*render)[i][j] = result;
                        imageFile->addPixel(i, j, result.x, result.y, result.z);
                    }
                }
                

                //Sphere Loop
                for (int obj = 0; obj < sphereList.size(); obj++)
                {
                    intersectionPoint = glm::vec3(0.0f);
                    if (curRay->raySphereIntersection(sphereList[obj], intersectionPoint, t0, t1, t2))
                    {
                        glm::uvec3 result = sphereList[obj]->material->shaderPixel(sphereList[obj]->surfaceNormal(intersectionPoint), lightList, curRay, lightBounces, intersectionPoint, triangleList, sphereList);
                        (*render)[i][j] = result;
                        imageFile->addPixel(i, j, result.x, result.y, result.z);
                    }
                }
            }
        }
        imageFile->fileName = "../output/" + std::to_string(seq+1) + ".tga";
        imageFile->fileWrite();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        renderTime += duration.count() / 1000.0f;
        std::cout << imageFile->fileName << ": FILE WRITTEN in " << (duration.count() / 1000.0f) << " seconds!" << std::endl;
        //std::cout << imageFile->fileName << ": FILE WRITTEN" << std::endl;

        //Basic Animation

        camPosition = glm::vec3(camPosition.x - 0.25f, camPosition.y, camPosition.z - 0.5f);
        camRotation = glm::vec3(-5.0f, 1.0f, -3.0f);
        cam->camRotate(camRotation.x, camRotation.y, camRotation.z);
        cam->camPos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);
    }
    std::cout << "Total Render Time: " << renderTime << " seconds!" << std::endl;
    return *render;
};
