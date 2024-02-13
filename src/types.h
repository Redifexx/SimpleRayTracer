#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <GLM/glm.hpp>
#include <cmath>
#include <math.h>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES

struct Light;
struct DirectionalLight;
struct Material;
struct Sphere;
struct Triangle;
struct Object;
struct Ray; 

glm::vec3 rgbToFloat(glm::uvec3 RGB)
{
    int rInt = glm::max(glm::uint(0), glm::min(glm::uint(255), RGB.x));
    int gInt = glm::max(glm::uint(0), glm::min(glm::uint(255), RGB.y));
    int bInt = glm::max(glm::uint(0), glm::min(glm::uint(255), RGB.z));

    float r = rInt / 255.0;
    float g = gInt / 255.0;
    float b = bInt / 255.0;

    return glm::vec3(r, g, b);
}
    
glm::uvec3 floatToRGB(glm::vec3 RGB)
{
    float rFlo = glm::max(0.0f, glm::min(1.0f, RGB.x));
    float gFlo = glm::max(0.0f, glm::min(1.0f, RGB.y));
    float bFlo = glm::max(0.0f, glm::min(1.0f, RGB.z));

    float r = int(rFlo * 255);
    float g = int(gFlo * 255);
    float b = int(bFlo * 255);

    return glm::uvec3(r, g, b);
}


glm::vec3 vectorToEuler(glm::vec3 direction)
{
    glm::vec3 angles;
    angles.y = atan2(direction.y, direction.x);
    angles.x = atan2(direction.z, sqrt(direction.x * direction.x + direction.y * direction.y));
    
    return angles;
}

glm::vec3 eulerToVector(glm::vec3 angles) //Asssumiong Radisans
{
    glm::vec3 dirVector;
    dirVector.x = cos(angles.y) * cos(angles.x);
    dirVector.y = sin(angles.y) * cos(angles.x);
    dirVector.z = sin(angles.x);
    //std::cout << dirVector.x << " " << dirVector.y << " " << dirVector.z << std::endl;
    return glm::normalize(dirVector);
}

struct Light 
{
    bool isPoint;
    glm::vec3 position;
    glm::vec3 direction;
    float strength;
    glm::vec3 baseColor;
    Light()
    {
        isPoint = false;
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
        strength = 0.1f;
        baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    void lightRotate(glm::vec3 rotation)
    {
        this->direction = eulerToVector(rotation);
    }

    glm::vec3 getDirection() 
    {
        return glm::normalize(this->direction);
    }

    glm::vec3 getDirection(glm::vec3 posA) 
    {
        glm::vec3 directionVector = posA - this->position;
        return glm::normalize(-directionVector);
    }
};


struct Material
{
    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    float diffuseIntensity;
    float ambientIntensity;
    float specularIntensity;
    float metallicFactor;
    float phongExponent;

    Material()
    {
        diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        ambientColor = diffuseColor;
        specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        diffuseIntensity = 0.5f;
        ambientIntensity = 0.15f;
        specularIntensity = 0.5f;
        metallicFactor = 0.5f;
        phongExponent = 8.0f;
    }

    glm::uvec3 shaderPixel(glm::vec3 surfaceNormal, std::vector<Light*> lights, Ray* camRay, int lightBounces, glm::vec3& intersectionPoint, std::vector<Triangle*>& triangles, std::vector<Sphere*>& spheres);
};

struct Sphere 
{
    glm::vec3 position;
    float radius;
    Material* material;
    Sphere()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 1.0;
        material = new Material();
    }
    Sphere(Material* mat)
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 1.0;
        material = mat;
    }
    ~Sphere()
    {
        delete material;
    }
    glm::vec3 surfaceNormal(glm::vec3 intersectionPoint)
    {
        //return (intersectionPoint - this->position) / this->radius;
        return glm::normalize(intersectionPoint - this->position);
    }
};

struct Triangle
{
    //Always drawn counter clockwise
    glm::vec3 vertices[3];
    Material* material;
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, Material* mat)
    {
        this->vertices[0] = a;
        this->vertices[1] = b;
        this->vertices[2] = c;
        material = mat;
    }
    glm::vec3 surfaceNormal()
    {
        glm::vec3 aVert = this->vertices[0];
        glm::vec3 bVert = this->vertices[1];
        glm::vec3 cVert = this->vertices[2];

        glm::vec3 vecAB = glm::vec3(bVert.x - aVert.x, bVert.y - aVert.y, bVert.z - aVert.z);
        glm::vec3 vecAC = glm::vec3(cVert.x - aVert.x, cVert.y - aVert.y, cVert.z - aVert.z);

        return glm::normalize(glm::cross(vecAB, vecAC));
    }
};

struct Object 
{
    glm::vec3 position;
    glm::vec3 rotation;
    std::vector<Triangle*> triangles;
    Material* material;
    Object(Material* mat)
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        material = mat;
    }
};

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    float scalar;
    Ray(float scalar_)
    {
        scalar = scalar_;
    }  

    bool rayPlaneIntersection(float depth, glm::vec3& intersectPoint, float& t0, float tMin, float tMax)
    {
        glm::vec3 planeCenter(0.0f, 0.0f, 0.0f);
        glm::vec3 planeNormal(0.0f, 1.0f, 0.0f);
        float t = -1.0f;
        float denom = glm::dot(planeNormal, this->direction);
        if (abs(denom) > 1e-6)
        {
            t = glm::dot(planeCenter - this->origin, planeNormal) / denom;
        }

        if ((t >= tMin) && (t <= tMax))
        {
            intersectPoint = this->origin + t * this->direction;
            //std::cout << "X: " << intersectPoint.x << " Y: " << intersectPoint.y << " Z: " << intersectPoint.z << std::endl;
            t0 = t;
            return true;
        }
        return false;
    }

    bool raySphereIntersection(Sphere* sphere, glm::vec3& intersectPoint, float& t0, float tMin, float tMax)
    {
        glm::vec3 originAndCenter = sphere->position - this->origin;
        bool sphereIntersection = false;
        float b = 2.0f *(glm::dot(this->direction, originAndCenter));
        float a = (glm::dot(this->direction, this->direction));
        float c = (glm::dot(originAndCenter, originAndCenter)) - std::pow(sphere->radius, 2);
        float discriminant = (std::pow(b, 2) - (4 * a * c));
        if (discriminant > 0.0)
        {
            float t1 = ((-b) + std::sqrt(discriminant)) / (2.0f *a);
            float t2 = ((-b) - std::sqrt(discriminant)) / (2.0f *a);
            float t = ((t1 < t2) ? t1 : t2);
            if ((t >= tMin) && (t <= tMax))
            {
                t0 = t;
                intersectPoint = this->origin + -t * this->direction;
                return true;
            }
            return false;
        }
        return false;
    }

    bool rayTriangleIntersection(Triangle* tri, glm::vec3& intersectPoint, float& t0, float t1, float t2)
    {
        glm::vec3 aVert = tri->vertices[0];
        glm::vec3 bVert = tri->vertices[1];
        glm::vec3 cVert = tri->vertices[2];

        //A - B Vertex
        float a = (aVert.x - bVert.x);
        float b = (aVert.y - bVert.y);
        float c = (aVert.z - bVert.z);

        //A - C Vertex
        float d = (aVert.x - cVert.x);
        float e = (aVert.y - cVert.y);
        float f = (aVert.z - cVert.z);

        //Direction Vector
        float g = (this->direction.x);
        float h = (this->direction.y);
        float i = (this->direction.z);

        //A - EyeOrigin
        float j = (aVert.x - this->origin.x);
        float k = (aVert.y - this->origin.y);
        float l = (aVert.z - this->origin.z);

        //Common Numbers
        float EIHF = (e * i) - (h * f);
        float GFDI = (g * f) - (d * i);
        float DHEG = (d * h) - (e * g);
        float AKJB = (a * k) - (j * b);
        float JCAL = (j * c) - (a * l);
        float BLKC = (b * l) - (k * c);
        float Mu = (a * (EIHF)) + (b * (GFDI)) + (c * (DHEG));

        float t = ((f * (AKJB)) + (e * (JCAL)) + (d * (BLKC))) / Mu;
        if ((t < t1) || (t > t2)) 
        {
            return false;
        }
        if (t > t0)
        {
            return false;
        } else
        {
            t0 = t;
        }
        //std::cout << t << std::endl;
        intersectPoint = this->origin + -t * this->direction;

        float Gamma = ((i * (AKJB)) + (h * (JCAL)) + (j * (BLKC))) / Mu;
        if ((Gamma < 0.0f) || (Gamma > 1.0f)) 
        {
            return false;
        }

        float Beta = ((j * (EIHF)) + (k * (GFDI)) + (l * (DHEG))) / Mu;
        if ((Beta < 0.0f) || (Beta > (1.0f - Gamma)) )
        {
            return false;
        }
        return true;
    }

};

//Material Shader
glm::uvec3 Material::shaderPixel(glm::vec3 surfaceNormal, std::vector<Light*> lights, Ray* camRay, int lightBounces, glm::vec3& intersectionPoint, std::vector<Triangle*>& triangles, std::vector<Sphere*>& spheres)
{
    surfaceNormal = glm::normalize(surfaceNormal);
    //Ambient
    glm::vec3 lightAmbient = 
    (
        (glm::vec3(ambientColor.x * ambientIntensity, ambientColor.y * ambientIntensity, ambientColor.z * ambientIntensity))
    );
    glm::vec3 lightSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightDiffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightVector;
    float t0 = 1000.0f;
    float t1 = 0.01f;
    float t2 = 1000.0f;

    for (int i = 0; i < lights.size(); i++)
    {
        float r = 1.0f;
        if (lights[i]->isPoint)
        {
            lightVector = lights[i]->getDirection(intersectionPoint);
            r = glm::distance(lights[i]->position, intersectionPoint);
        }
        else
        {
            lightVector = -lights[i]->getDirection();
        }

        //Diffuse
        float diffuseMultipliers = (diffuseIntensity) * (lights[i]->strength / (r*r)) * glm::max(0.0f, (glm::dot(surfaceNormal, lightVector)));
        glm::uvec3 diffuseVectors = 
        (
            (glm::uvec3(diffuseColor.x, diffuseColor.y, diffuseColor.z)
            *
            glm::uvec3(lights[i]->baseColor.x, lights[i]->baseColor.y, lights[i]->baseColor.z))
        );

        //Specular
        glm::vec3 eyeVector = glm::normalize(camRay->direction);
        glm::vec3 eyePlusLight = (eyeVector + lightVector);
        glm::vec3 hVector = glm::normalize(eyePlusLight / glm::length(eyePlusLight));

        float specularMultipliers = (specularIntensity) * (lights[i]->strength / (r*r)) * std::pow(glm::max(0.0f, (glm::dot(surfaceNormal, hVector))), phongExponent);

        glm::uvec3 specularVectors = 
        (
            (glm::uvec3(specularColor.x, specularColor.y, specularColor.z)
            *
            glm::uvec3(lights[i]->baseColor.x, lights[i]->baseColor.y, lights[i]->baseColor.z))
        );  
        
        Ray* shadowRay = new Ray(1000.0f);
        shadowRay->origin = intersectionPoint;
        shadowRay->direction = -lightVector;
        glm::vec3 dummyIntersect = glm::vec3(0.0f, 0.0f, 0.0f);
        bool isBlocked = false;
        
        for (int count = 0; count < triangles.size(); count++)
        {
            if (shadowRay->rayTriangleIntersection(triangles[count], dummyIntersect, t0, t1, t2))
            {
                //isBlocked = true;
                break;
            }
        }
        

        for (int count = 0; count < spheres.size(); count++)
        {
            if (shadowRay->raySphereIntersection(spheres[count], dummyIntersect, t0, t1, t2))
            {
                isBlocked = true;
                break;
            }
        }
        

        if (!isBlocked)
        {
            lightDiffuse +=
            (
                glm::vec3(diffuseVectors.x * diffuseMultipliers, diffuseVectors.y * diffuseMultipliers, diffuseVectors.z * diffuseMultipliers)
            );

            lightSpecular +=
            (
                glm::vec3(specularVectors.x * specularMultipliers, specularVectors.y * specularMultipliers, specularVectors.z * specularMultipliers)
            );
        }
    }

    glm::vec3 finalColor = (lightDiffuse + lightAmbient + lightSpecular);
    Ray* tempRay = new Ray(1000.0f);
    tempRay->origin = intersectionPoint;
    tempRay->direction = glm::reflect(camRay->direction, surfaceNormal);
    glm::vec3 newIntersection;

    if (lightBounces > 0)
    {
        for (int count = 0; count < spheres.size(); count++)
        {
            if (tempRay->raySphereIntersection(spheres[count], newIntersection, t0, t1, t2))
            {   
                //std::cout << "SPHERE HIT" << std::endl;
                glm::vec3 reflection = rgbToFloat((this->shaderPixel(spheres[count]->surfaceNormal(newIntersection), lights, tempRay, lightBounces - 1, newIntersection, triangles, spheres)));
                finalColor += (metallicFactor * reflection * spheres[count]->material->diffuseColor);
                //std::cout << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;
            }
        }

        for (int count = 0; count < triangles.size(); count++)
        {
            if (tempRay->rayTriangleIntersection(triangles[count], newIntersection, t0, t1, t2))
            {
                glm::vec3 reflection = rgbToFloat((this->shaderPixel(triangles[count]->surfaceNormal(), lights, tempRay, lightBounces - 1, newIntersection, triangles, spheres)));
                finalColor += (metallicFactor * reflection * triangles[count]->material->diffuseColor);
            }
        }
    }
    delete tempRay;
    
    glm::vec3 clampedEmission 
    (
        glm::clamp(finalColor.x, 0.0f, 1.0f),
        glm::clamp(finalColor.y, 0.0f, 1.0f),
        glm::clamp(finalColor.z, 0.0f, 1.0f)
    );
    //return finalColor;
    return floatToRGB(clampedEmission);
    //return floatToRGB(finalColor);
}


#endif // TYPES_H
