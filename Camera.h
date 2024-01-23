#pragma once
#include <vector>
#include "types.h"
using namespace std;

struct PerspectiveCam
{
    VectorThree e;
    VectorThree u;
    VectorThree v;
    VectorThree w;
    float projectionDistance;
    Ray* getRay(int x, int y);
    //vector<Ray*> cameraRays;

};