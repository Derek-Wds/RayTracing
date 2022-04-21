#ifndef RAY_HEADER_H
#define RAY_HEADER_H

#include "Vec3.hpp"

// Ray class.
class Ray
{
public:
    Vec3 orig;
    Vec3 dir;
    Ray() = default;
    Ray(const Vec3 &origin, const Vec3 &direction)
        : orig(origin), dir(direction)
    {
    }

    Vec3 at(double t) const
    {
        return orig + t * dir;
    }
};

#endif