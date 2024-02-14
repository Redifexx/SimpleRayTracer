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
    red->diffuseColor = rgbToFloat(glm::uvec3(255, 255, 255));
    red->ambientColor = rgbToFloat(glm::uvec3(255, 255, 255));
    red->diffuseIntensity = 0.0f;
    red->specularIntensity = 1.0f;
    red->metallicFactor = 0.0f;

    Material* green = new Material();
    green->diffuseColor = rgbToFloat(glm::uvec3(0, 255, 0));
    green->ambientColor = rgbToFloat(glm::uvec3(0, 255, 0));

    Material* blue = new Material();
    blue->diffuseColor = rgbToFloat(glm::uvec3(0, 0, 255));
    blue->ambientColor = rgbToFloat(glm::uvec3(0, 0, 255));

    Material* magenta = new Material();
    magenta->diffuseColor = rgbToFloat(glm::uvec3(255, 255, 255));
    magenta->ambientColor = rgbToFloat(glm::uvec3(255, 255, 255));
    magenta->diffuseIntensity = 0.8f;
    magenta->specularIntensity = 0.2f;
    magenta->metallicFactor = 0.8f;


    Material* orange = new Material();
    orange->diffuseColor = rgbToFloat(glm::uvec3(255, 0, 0));
    orange->ambientColor = rgbToFloat(glm::uvec3(255, 0, 0));
    orange->diffuseIntensity = 1.0f;
    orange->specularIntensity = 0.0f;
    orange->metallicFactor = 0.0f;

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
    glossy->metallicFactor = 0.0f;

    Material* diffuse = new Material();
    diffuse->diffuseIntensity = 1.0f;
    diffuse->specularIntensity = 0.0f;
    diffuse->metallicFactor = 0.0f;

    Material* mirror = new Material();
    mirror->diffuseIntensity = 0.0f;
    mirror->specularIntensity = 0.1f;
    mirror->metallicFactor = 1.0f;


    //
    //Pyramid Points
    glm::vec3 tetraPos(0.0f, 0.1f, 0.0f);
    float triScale = 2.0f;
    glm::vec3 aVertex = glm::vec3((tetraPos.x + -1.0f) * triScale, tetraPos.y * triScale, (tetraPos.z + 1.0f) * triScale);
    glm::vec3 bVertex = glm::vec3((tetraPos.x + 1.0f) * triScale, tetraPos.y * triScale, (tetraPos.z + 1.0f) * triScale);
    glm::vec3 cVertex = glm::vec3((tetraPos.x + 1.0f) * triScale, tetraPos.y * triScale, (tetraPos.z + -1.0f) * triScale);
    glm::vec3 dVertex = glm::vec3((tetraPos.x + -1.0f) * triScale, tetraPos.y * triScale, (tetraPos.z + -1.0f) * triScale);
    glm::vec3 eVertex = glm::vec3((tetraPos.x + 0.0f) * triScale, (tetraPos.y + 2.0f) * triScale, (tetraPos.z + 0.0f) * triScale);

    float triRotation = 0.0f;
    float triRotationRad = glm::radians(triRotation);
    glm::mat4 triRotMatrix = glm::rotate(glm::mat4(1.0f), triRotationRad, glm::vec3(0.0f, 1.0f, 0.0f));
    //Tri rotation
    aVertex = glm::vec3(triRotMatrix * glm::vec4(aVertex, 1.0f));
    bVertex = glm::vec3(triRotMatrix * glm::vec4(bVertex, 1.0f));
    cVertex = glm::vec3(triRotMatrix * glm::vec4(cVertex, 1.0f));
    dVertex = glm::vec3(triRotMatrix * glm::vec4(dVertex, 1.0f));
    eVertex = glm::vec3(triRotMatrix * glm::vec4(eVertex, 1.0f));


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
    sphere = new Sphere(orange);
    sphere->position = glm::vec3(-4.0, 4.0, 0.0);
    sphere->radius = 1.0f;
    sphereList.push_back(sphere);

    //Sphere - Mirror
    sphere = new Sphere(mirror);
    sphere->position = glm::vec3(-15.0, 5.0, -15.0);
    sphere->radius = 10.0f;
    sphereList.push_back(sphere);

    //Sphere - Diffuse
    sphere = new Sphere(red);
    sphere->position = glm::vec3(4.0, 4.0, 0.0);
    sphere->radius = 1.0f;
    sphereList.push_back(sphere);

    //Sphere - Diffuse
    sphere = new Sphere(green);
    sphere->position = glm::vec3(0.0, 4.0, 4.0);
    sphere->radius = 1.0f;
    sphereList.push_back(sphere);

    //Sphere - Diffuse
    sphere = new Sphere(blue);
    sphere->position = glm::vec3(0.0, 4.0, -4.0);
    sphere->radius = 1.0f;
    sphereList.push_back(sphere);

    //
    //Lights

    //Directional - Straight Down
    Light* light = new Light();
    light->strength = 1.0f;
    light->lightRotate(glm::vec3(glm::radians(180.0f), glm::radians(45.0f), glm::radians(10.0f)));
    lightList.push_back(light);

    //Point1
    Light* redLight = new Light();
    redLight->strength = 15.0f;
    redLight->position = glm::vec3(-2.5f, 1.0f, -5.0f);
    redLight->baseColor = glm::vec3(1.0f, 0.0f, 0.0f);
    redLight->isPoint = true;
    lightList.push_back(redLight);
    
    //Point 2
    Light* greenLight = new Light();
    greenLight->strength = 15.0f;
    greenLight->position = glm::vec3(2.5f, 6.0f, -5.0f);
    greenLight->baseColor = glm::vec3(0.0f, 1.0, 0.0f);
    greenLight->isPoint = true;
    lightList.push_back(greenLight);

    //Point 3
    Light* blueLight = new Light();
    blueLight->strength = 15.0f;
    blueLight->position = glm::vec3(0.0f, 6.0f, 5.0f);
    blueLight->baseColor = glm::vec3(0.0f, 0.0, 1.0f);
    blueLight->isPoint = true;
    lightList.push_back(blueLight);

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
    triangle = new Triangle(bVertex, aVertex, eVertex, magenta);
    triangleList.push_back(triangle);

    triangle = new Triangle(cVertex, bVertex, eVertex, magenta);
    triangleList.push_back(triangle);

    triangle = new Triangle(dVertex, cVertex, eVertex, magenta);
    triangleList.push_back(triangle);

    triangle = new Triangle(aVertex, dVertex, eVertex, magenta);
    triangleList.push_back(triangle);

    triangle = new Triangle(bVertex, cVertex, aVertex, magenta);
    triangleList.push_back(triangle);

    triangle = new Triangle(cVertex, dVertex, aVertex, magenta);
    triangleList.push_back(triangle);
    
    //
    //Camera

    glm::vec3 camPosition = glm::vec3(15.0f, 3.0f, 15.0f);
    //glm::vec3 camPosition = glm::vec3(0.0f, 20.0f, 20.0f);
    cam->camPos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);

    //(L-R)
    glm::vec3 camRotation = glm::vec3(135.0f, 0.0f, -90.0f);
    //glm::vec3 camRotation = glm::vec3(180.0f, 0.0f, -90.0f);
    cam->camRotate(camRotation.x, camRotation.y, camRotation.z);

    cam->isPerspective = !isOrtho;
    //cam->isPerspective = false;
    //cam->orthoScale = 10.0f;
    cam->perspectiveDistance = 5.0f;
    //cam->perspectiveDistance = 30.0f;
    
    //Camera Animation

    //
    //Additional Settings
    glm::vec3 worldOrigin(0.0f, 0.0f, 0.0f);
    float orbitRadius = 7.0f;
    float orbitSpeed = 0.7f;
    int frames = 1;
    float zPosMovement = 30.0f / frames;
    float camRotMovement = 90.0f / frames;
    float perspectiveMovement = 25.0f / (frames / 2.0f);

    //Reflection Depth
    int lightBounces = 2;

    //Infinite Plane Specification
    float planeDepth = -1.0f; 

    //Seq is how many rendered frames
    for (int seq = 0; seq < frames; seq++)
    {
        //Light Orbit
        float angle = (2.0f * glm::pi<float>() * orbitSpeed * (seq + 1)) / frames;
        float rx = worldOrigin.x + orbitRadius * glm::cos(angle * (glm::pi<float>() / 6));
        float rz = worldOrigin.z + orbitRadius * glm::sin(angle * (glm::pi<float>() / 6));
        float gx = worldOrigin.x + orbitRadius * glm::cos(angle * 5 * (glm::pi<float>() / 6));
        float gz = worldOrigin.z + orbitRadius * glm::sin(angle * 5 * (glm::pi<float>() / 6));
        float bx = worldOrigin.x + orbitRadius * glm::cos(angle * 3 * (glm::pi<float>() / 2));
        float bz = worldOrigin.z + orbitRadius * glm::sin(angle * 3 * (glm::pi<float>() / 2));
        redLight->position = glm::vec3(rx, 6.0f, rz);
        greenLight->position = glm::vec3(gx, 6.0f, gz);
        blueLight->position = glm::vec3(bx, 6.0f, bz);
        
        if ((seq) == (frames / 2))
        {
            perspectiveMovement = -perspectiveMovement;
            std::cout << "PM: " << perspectiveMovement << std::endl;
        }

        auto start = std::chrono::high_resolution_clock::now();
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
            
            }
        }
        imageFile->fileName = "../output/" + std::to_string(seq+1) + ".tga";
        imageFile->fileWrite();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        renderTime += duration.count() / 1000.0f;
        std::cout << imageFile->fileName << ": FILE WRITTEN in " << (duration.count() / 1000.0f) << " seconds!" << std::endl;

        //Basic Cam Animation
        std::cout << cam->perspectiveDistance << std::endl;
        cam->perspectiveDistance += perspectiveMovement;
        camPosition = glm::vec3(camPosition.x, camPosition.y, camPosition.z - zPosMovement);
        camRotation = glm::vec3(-camRotMovement, 0.0, 0.0f);
        cam->camRotate(camRotation.x, camRotation.y, camRotation.z);
        cam->camPos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);
    }
    std::cout << "Total Render Time: " << renderTime << " seconds!" << std::endl;
    return *render;
};
