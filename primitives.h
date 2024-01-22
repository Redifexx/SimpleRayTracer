#pragma once
#include <vector>
#include "types.h"
using namespace std;

struct Sphere
{
    VectorThree* position;
    float radius;
    
};

Sphere::Sphere()
{
    position->x = 0.0f;
    position->y = 0.0f;
    position->z = 0.0f;
    radius = 1.0f;
};