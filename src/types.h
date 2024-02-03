#include <GLM/glm.hpp>
#include <math.h>

struct Sphere 
{
    glm::vec3 position;
    float radius;
    Sphere()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        radius = 1.0;
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

    /*
    bool raySphereIntersection(Ray* camRay, Sphere* sphere)
    {
        float distance = glm::dot((sphere->position - camRay->origin), camRay->direction);
        if (distance < 0.0f)
        {
            return false;
        }
        float distance2 = sqrt((distance*distance))
    }
    */
};
