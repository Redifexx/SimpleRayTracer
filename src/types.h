#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <GLM/glm.hpp>
#include <cmath>
#include <math.h>
#define _USE_MATH_DEFINES

struct Light;
struct DirectionalLight;
struct Material;
struct Sphere;
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



struct Light 
{
    public:
    glm::vec3 position;
    glm::vec3 direction;
    float strength;
    glm::vec3 baseColor;
    Light()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
        strength = 1.0f;
        baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    virtual glm::vec3 getDirection() = 0;
};

struct DirectionalLight : public Light
{
    glm::vec3 getDirection() override { return this->direction; }
};

struct Material
{
    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    float diffuseIntensity;
    float ambientIntensity;
    float specularIntensity;
    float phongExponent;

    Material()
    {
        diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        ambientColor = diffuseColor;
        specularColor = diffuseColor;
        diffuseIntensity = 1.0f;
        ambientIntensity = 0.15f;
        specularIntensity = 1.0f;
        phongExponent = 100.0f;
    }

    glm::uvec3 shaderPixel(glm::vec3 surfaceNormal, Light* light, glm::vec3 camVector)
    {
        glm::vec3 minClamp(0.0f, 0.0f, 0.0f);
        glm::vec3 maxClamp(1.0f, 1.0f, 1.0f);
        //Ambient
        glm::vec3 lightAmbient = 
        (
            (glm::vec3(ambientColor.x * ambientIntensity, ambientColor.y * ambientIntensity, ambientColor.z * ambientIntensity))
        );

        //Diffuse
        float diffuseMultipliers = (diffuseIntensity) * (light->strength) * glm::max(0.0f, (glm::dot(surfaceNormal, light->getDirection())));
        glm::uvec3 diffuseVectors = 
        (
            (glm::uvec3(diffuseColor.x, diffuseColor.y, diffuseColor.z)
            *
            glm::uvec3(light->baseColor.x, light->baseColor.y, light->baseColor.z))
        );

        glm::vec3 lightDiffuse =
        (
            glm::vec3(diffuseVectors.x * diffuseMultipliers, diffuseVectors.y * diffuseMultipliers, diffuseVectors.z * diffuseMultipliers)
        );


        //Specular
        glm::vec3 eyeVector = glm::normalize(-camVector);
        glm::vec3 hVector = (eyeVector + glm::normalize(light->getDirection())) / glm::length(eyeVector + glm::normalize(light->getDirection()));

        float specularMultipliers = (specularIntensity) * (light->strength) * std::pow(glm::max(0.0f, (glm::dot(eyeVector, hVector))), phongExponent);

        glm::uvec3 specularVectors = 
        (
            (glm::uvec3(specularColor.x, specularColor.y, specularColor.z)
            *
            glm::uvec3(light->baseColor.x, light->baseColor.y, light->baseColor.z))
        );

        glm::vec3 lightSpecular =
        (
            glm::vec3(specularColor.x * specularMultipliers, specularColor.x * specularMultipliers, specularColor.x * specularMultipliers)
        );

        


        //Diffuse
        //float diffuseEmission = (diffuseCoefficient) * (light->strength) * glm::max(0.0f, (glm::dot(surfaceNormal, light->getDirection())));
        //glm::uvec3 diffuseColor = glm::uvec3(baseColor.x * diffuseEmission, baseColor.y * diffuseEmission, baseColor.z * diffuseEmission);
        //glm::uvec3 ambientEmission = glm::uvec3(baseColor.x * ambientCoefficient, baseColor.y * ambientCoefficient, baseColor.z * ambientCoefficient);
        //std::cout << diffuseEmission << std::endl;
        glm::vec3 finalColor = lightDiffuse + lightAmbient + lightSpecular;
        
        //glm::uvec3 minClamp(0, 0, 0);
        //glm::uvec3 maxClamp(255, 255, 255);
        glm::vec3 clampedEmission 
        (
            glm::clamp(finalColor.x, minClamp.x, maxClamp.x),
            glm::clamp(finalColor.y, minClamp.y, maxClamp.y),
            glm::clamp(finalColor.z, minClamp.z, maxClamp.z)
        );
        //return finalColor;
        return floatToRGB(clampedEmission);
    }
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
        return (intersectionPoint - this->position) / this->radius;
        //return 2.0f * (intersectionPoint - this->position);
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
    bool raySphereIntersection(Sphere* sphere, glm::vec3& intersectPoint, glm::mat4 camViewMatrix)
    {
        glm::vec3 originAndCenter = this->origin - sphere->position;
        bool sphereIntersection = false;
        float b = (glm::dot(this->direction, originAndCenter));
        float a = (glm::dot(this->direction, this->direction));
        float c = (glm::dot(originAndCenter, originAndCenter)) - std::pow(sphere->radius, 2);
        float discriminant = (std::pow(b, 2) - a*c);
        if (discriminant > 0.0)
        {
            float t1 = ((-b) - std::sqrt(discriminant)) / a;
            float t2 = ((-b) + std::sqrt(discriminant)) / a;
            float t = (t1 < t2) ? t1 : t2;
            intersectPoint = this->origin + t * this->direction;
            return true;
        }
        return false;
    }
};
#endif // TYPES_H
