/*
#include "types.h"
#include <iostream>

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
        float diffuseEmission = (diffuseCoefficient) * (light->strength) * glm::max(0.0f, (glm::dot(surfaceNormal, light->getDirection())));
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

*/