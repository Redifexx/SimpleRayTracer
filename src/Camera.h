#include "types.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

struct Camera
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
    bool isPerspective;

    Camera(int screenWidth_, int screenHeight_)
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
        isPerspective = true;
        updateViewMatrix();
    }

    void camRotate(float rotX, float rotY, float rotZ)
    {
        glm::quat rotationX = glm::angleAxis(glm::radians(rotX), camBasisU);
        glm::quat rotationY = glm::angleAxis(glm::radians(rotY), camBasisV);
        glm::quat rotationZ = glm::angleAxis(glm::radians(rotZ), camBasisW);

        glm::quat rotation = rotationZ * rotationY * rotationX;

        camBasisU = rotation * camBasisU;
        camBasisV = rotation * camBasisV;
        camBasisW = rotation * camBasisW;

        //camBasisU = glm::normalize(camBasisU);
        //camBasisU = glm::normalize(camBasisV);
        //camBasisU = glm::normalize(camBasisW);

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

    void togglePerspective()
    {
        isPerspective = !isPerspective;
    }

    Ray* getRay(int ic, int jc)
    {
        float u = (ic + 0.5) / screenWidth;
        float v = (jc + 0.5) / screenHeight;
        Ray* curRay = new Ray(maxRayLength);
        if (isPerspective)
        {
            curRay->origin = ((camPos));
            glm::vec3 tempW = camBasisW;
            tempW *= -perspectiveDistance;
            glm::vec3 tempU = camBasisU;
            tempU *= (u - 0.5f);
            glm::vec3 tempV = camBasisV;
            tempV *= (v - 0.5f);
            curRay->direction = tempW + tempU + tempV;
            curRay->direction = glm::vec3(viewMatrix * glm::vec4(curRay->direction, 0.0));
        } else
        {
            glm::vec3 tempW = camBasisW;
            curRay->direction = ((-tempW));
            //curRay->direction = glm::vec3(viewMatrix * glm::vec4(curRay->direction, 0.0));
            glm::vec3 tempU = camBasisU;
            tempU *= (u - 0.5f);
            glm::vec3 tempV = camBasisV;
            tempV *= (v - 0.5f);
            curRay->origin = tempW + tempU + tempV;
            curRay->origin = glm::vec3(viewMatrix * glm::vec4(curRay->origin, 0.0));
        }
        return curRay;
    }
};