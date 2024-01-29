#include "Camera.h"

VectorThree addVectors(VectorThree a, VectorThree b);
void scaleVectors(float scalar, VectorThree& a);

PerspectiveCam::PerspectiveCam()
{
    e.x = 0.5;
    e.y = 0.5;
    e.z = 0;

    u.x = 1;
    u.y = 0;
    u.z = 0;

    v.x = 0;
    v.y = 1;
    v.z = 0;

    w.x = 0;
    w.y = 0;
    w.z = 1;

    projectionDistance = 5.0f;
}

Ray* PerspectiveCam::getRay(int x, int y)
{
    x += 1;
    y += 1;


    Ray* curRay = new Ray;
    curRay->origin.x = this->e.x;
    curRay->origin.y = this->e.y;
    curRay->origin.z = this->e.z;

    VectorThree* tempU = new VectorThree;
    VectorThree* tempV = new VectorThree;
    VectorThree* tempW = new VectorThree;

    tempW->x = -projectionDistance * w.x;
    tempW->y = -projectionDistance * w.y;
    tempW->z = -projectionDistance * w.z;

    tempU->x = x * u.x;
    tempU->y = x * u.y;
    tempU->z = x * u.z;

    tempV->x = y * v.x;
    tempV->y = y * v.y;
    tempV->z = y * v.z;

    VectorThree finalVec = addVectors(*tempW, (addVectors(*tempU, *tempV)));
    delete tempW, tempU, tempV;
    curRay->direction.x = finalVec.x;
    curRay->direction.y = finalVec.y;
    curRay->direction.z = finalVec.z;
    return curRay;
}

VectorThree addVectors(VectorThree a, VectorThree b)
{
    VectorThree curVec;
    curVec.x = a.x + b.x;
    curVec.y = a.y + b.y;
    curVec.z = a.z + b.z;
    return curVec;
}

void scaleVectors(float scalar, VectorThree& a)
{
    a.x *= scalar;
    a.y *= scalar;
    a.z *= scalar;
}