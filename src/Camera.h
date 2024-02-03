#include "object.h"

struct PerspectiveCam
{
    glm::vec3 camPos;
    glm::vec3 camBasisU;
    glm::vec3 camBasisV;
    glm::vec3 camBasisW;
    float persectiveDistance;
    float maxRayLength;
    int screenWidth;
    int screenHeight;
    PerspectiveCam(int screenWidth_, int screenHeight_)
    {
        camPos = glm::vec3(0.0f, 0.0f, -5.0f);
        camBasisU = glm::vec3(0.5f, 0.0f, 0.0f);
        camBasisV = glm::vec3(0.0f, 0.5f, 0.0f);
        camBasisW = glm::vec3(0.0f, 0.0f, 1.0f);
        persectiveDistance = 1.0;
        maxRayLength = 2000;
        screenWidth = screenWidth_;
        screenHeight = screenHeight_;
    }

    Ray* getRay(int ic, int jc)
    {
        float u = (ic + 0.5) / screenWidth;
        float v = (jc + 0.5) / screenHeight;
        Ray* curRay = new Ray(maxRayLength);
        curRay->origin = ((camPos));
        glm::vec3 tempW = camBasisW;
        tempW *= -persectiveDistance;
        glm::vec3 tempU = camBasisU;
        tempU *= (u - 0.5f);
        glm::vec3 tempV = camBasisV;
        tempV *= (v - 0.5f);
        curRay->direction = (tempW + tempU) + (tempV);
        return curRay;
    }
};