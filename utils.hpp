#ifndef UTIL_HEADER_H
#define UTIL_HEADER_H

#include "Vec3.hpp"

#include <cmath>

const double pi = 3.1415926535897932385;

// Degree to radian conversion.
inline double DegtoRad(double degrees)
{
    return degrees * pi / 180.0;
};

// Generate random double values between min and max.
inline double RandDouble(double min = 0.0, double max = 1.0)
{
    return min + rand() / (RAND_MAX / (max - min));
};

// Generate random vectors values between min and max.
inline Vec3 RandVec(double min = 0.0, double max = 1.0)
{
    return Vec3(RandDouble(min, max), RandDouble(min, max), RandDouble(min, max));
};

// Generate random unit vectors.
inline Vec3 RandUnitVec()
{
    const double a = RandDouble(0, 2 * pi);
    const double z = RandDouble(-1, 1);
    const double r = sqrt(1 - z * z);

    return Vec3(r * std::cos(a), r * std::sin(a), z);
};

// Generate random vectors within the unit sphere.
Vec3 RandVecInUnitSphere()
{
    while (true)
    {
        Vec3 v = RandVec(-1.0, 1.0);
        if (v.length() >= 1.0)
            continue;
        return v;
    }
};

// Generate random vectors within the unit disk.
Vec3 RandVecInUnitDisk()
{
    while (true)
    {
        Vec3 v = Vec3(RandDouble(-1, 1), RandDouble(-1, 1), 0);
        if (v.length() >= 1.0)
            continue;
        return v;
    }
};

// Get the texture coordinates of the sphere.
inline void GetSphereUV(const Vec3 &p, double &u, double &v)
{
    const auto phi = atan2(p.z, p.x);
    const auto theta = asin(p.y);
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
};

#endif