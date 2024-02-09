#include <GLM/glm.hpp>
#include <cmath>
#include <math.h>
#define _USE_MATH_DEFINES

class Light 
{
    public:
    glm::vec3 position;
    glm::vec3 direction;
    float strength;
    glm::uvec3 baseColor;
    Light()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        direction = glm::vec3(0.0f, -1.0f, 0.0f);
        strength = 1.0f;
        baseColor = glm::uvec3(255, 255, 255);
    }
    virtual glm::vec3 getDirection() = 0;
};

struct DirectionalLight : public Light
{
    glm::vec3 getDirection()
    {
        return this->direction;
    }

};

struct Material
{
    glm::uvec3 baseColor;
    float ambientCoefficient;
    glm::uvec3 ambientColor;
    float diffuseCoefficient;
    float specularCoefficient;
    Material()
    {
        baseColor = glm::uvec3(255, 0, 0);
        ambientCoefficient = 0.15f;
        ambientColor = glm::uvec3(200, 200, 200);
        diffuseCoefficient = 0.5f;
        specularCoefficient = 0.5f;
    }

    glm::uvec3 shaderPixel(glm::vec3 surfaceNormal, Light* light)
    {
        //Diffuse
        float diffuseEmission = (diffuseCoefficient) * (light->strength) * glm::max(0.0f, (glm::dot(surfaceNormal, -light->getDirection())));
        glm::uvec3 diffuseColor = glm::uvec3(baseColor.x * diffuseEmission, baseColor.y * diffuseEmission, baseColor.z * diffuseEmission);
        glm::uvec3 ambientEmission = glm::uvec3(baseColor.x * ambientCoefficient, baseColor.y * ambientCoefficient, baseColor.z * ambientCoefficient);
        //std::cout << diffuseEmission << std::endl;
        glm::uvec3 finalColor = diffuseColor + ambientEmission;
        
        glm::uvec3 minClamp(0, 0, 0);
        glm::uvec3 maxClamp(255, 255, 255);
        glm::uvec3 clampedEmission 
        (
            glm::clamp(finalColor.x, minClamp.x, maxClamp.x),
            glm::clamp(finalColor.y, minClamp.y, maxClamp.y),
            glm::clamp(finalColor.z, minClamp.z, maxClamp.z)
        );
        return clampedEmission;
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
        return glm::normalize(intersectionPoint - this->position);
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
        glm::mat4 inverseViewMatrix = glm::inverse(camViewMatrix);
        glm::vec3 originWorld = glm::vec3(inverseViewMatrix * glm::vec4(this->origin, 1.0f));
        glm::vec3 directionWorld = glm::vec3(inverseViewMatrix * glm::vec4(this->direction, 0.0));
        glm::vec3 oc = originWorld - sphere->position;
        bool sphereIntersection = false;
        float a = (glm::dot(directionWorld, directionWorld));
        float b = (2.0f * glm::dot(oc, directionWorld));
        float c = (glm::dot(oc, oc)) - std::pow(sphere->radius, 2);
        float discriminant = (std::pow(b, 2) - 4*a*c);
        if (discriminant > 0.0)
        {
            float t1 = ((-b) - std::sqrt(discriminant)) / 2*a;
            float t2 = ((-b) + std::sqrt(discriminant)) / 2*a;
            float t = (t1 < t2) ? t1 : t2;
            intersectPoint = originWorld + t * directionWorld;
            return true;
        }
        return false;
    }
};

