#ifndef HR_HEADER_H
#define HR_HEADER_H

#include "Ray.hpp"
#include "Vec3.hpp"

struct HitRecord {
    float t;
    double u;
    double v;
    Vec3 p;
    Vec3 normal;
    bool front = false;
};

#endif