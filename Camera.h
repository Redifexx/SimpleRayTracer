#pragma once
#include <vector>
#include "types.h"
using namespace std;

struct PerspectiveCam
{
    VectorThree u;
    VectorThree v;
    VectorThree w;
    float projectionDistance;
    vector<Ray*> cameraRays;

};