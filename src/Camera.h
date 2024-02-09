#include "types.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

struct PerspectiveCam
{
    glm::vec3 camPos;
    glm::vec3 camBasisU;
    glm::vec3 camBasisV;
    glm::vec3 camBasisW;
    glm::quat orientation;
    float perspectiveDistance;
    float maxRayLength;
    int screenWidth;
    int screenHeight;
    glm::mat4 viewMatrix;

    PerspectiveCam(int screenWidth_, int screenHeight_)
    {
        camPos = glm::vec3(0.0f, 0.0f, 0.0f);
        camBasisU = glm::vec3(1.0f, 0.0f, 0.0f);
        camBasisV = glm::vec3(0.0f, 1.0f, 0.0f);
        camBasisW = glm::vec3(0.0f, 0.0f, 1.0f);
        orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        perspectiveDistance = 1.0f;
        maxRayLength = 1000;
        screenWidth = screenWidth_;
        screenHeight = screenHeight_;
        updateViewMatrix();
    }

    void camRotate(float rotX, float rotY, float rotZ)
    {
        glm::quat rotation = glm::quat(glm::vec3(rotX, rotY, rotZ));
        orientation = rotation * orientation;
        orientation = glm::normalize(orientation);
        updateViewMatrix();
    }

    void updateViewMatrix()
    {
        glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
        glm::vec3 direction = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
        glm::vec3 up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

        viewMatrix = glm::lookAt(camPos, camPos + direction, up);
    }

    float calculateFOV()
    {
        return glm::degrees(2.0f * atanf((this->screenHeight / 2.0f) / this->perspectiveDistance));
    }

    void setFOV(float fov)
    {
        float halfFov = glm::radians(fov / 2.0f);
        perspectiveDistance = (screenHeight / 2.0f) / tanf(halfFov);
    }


    Ray* getRay(int ic, int jc)
    {
        float u = (ic + 0.5) / screenWidth;
        float v = (jc + 0.5) / screenHeight;
        Ray* curRay = new Ray(maxRayLength);
        curRay->origin = ((camPos));
        glm::vec3 tempW = camBasisW;
        tempW *= -perspectiveDistance;
        glm::vec3 tempU = camBasisU;
        tempU *= (u - 0.5f);
        glm::vec3 tempV = camBasisV;
        tempV *= (v - 0.5f);
        curRay->direction = tempW + tempU + tempV;
        curRay->direction = glm::vec3(viewMatrix * glm::vec4(curRay->direction, 0.0));
        return curRay;
    }
};