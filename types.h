#pragma once
#include <vector>
using namespace std;

struct VectorThree
{
    float x;
    float y;
    float z;
};

struct Ray
{
    float t;
    VectorThree origin;
    VectorThree direction;
};

struct UnitSphere
{
    VectorThree position;
};