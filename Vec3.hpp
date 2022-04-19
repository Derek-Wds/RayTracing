#ifndef VEC_HEADER_H
#define VEC_HEADER_H

#include <cmath>
#include <iostream>

class Vec3 {
    public:
        double x, y, z;
        Vec3(double xx = 0.0, double yy = 0.0, double zz = 0.0) : x(xx), y(yy), z(zz) {}

        Vec3 operator+(const double t) { return Vec3(x + t, y + t, z + t); }
        Vec3 operator-(const double t) { return Vec3(x - t, y - t, z - t); }
        Vec3 operator*(const double t) { return Vec3(x * t, y * t, z * t); }
        Vec3 operator/(const double t) { return Vec3(x / t, y / t, z / t); }

        Vec3& operator+=(const Vec3 &vec) {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            return *this;
        }

        Vec3& operator+=(const double t) {
            x += t;
            y += t;
            z += t;
            return *this;
        }
        Vec3 operator-() const { return Vec3(-x, -y, -z); }
        Vec3& operator*=(const double t) {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }
        Vec3& operator/=(const double t) {
            return *this *= 1 / t;
        }

        Vec3 operator+(const Vec3 &vec) const {
            return Vec3(x + vec.x, y + vec.y, z + vec.z);
        }

        Vec3 operator-(const Vec3 &vec) const {
            return Vec3(x - vec.x, y - vec.y, z - vec.z);
        }

        Vec3 operator*(const Vec3 &vec) const {
            return Vec3(x * vec.x, y * vec.y, z * vec.z);
        }

        double length() const {
            return sqrt(x * x + y * y + z * z);
        }

        double length_squared() const {
            return x * x + y * y + z * z;
        }
};


// Vec3 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
};

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t*v.x, t*v.y, t*v.z);
};

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
};

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
};

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
};

inline Vec3 normalize(Vec3 v) {
    return v / v.length();
};

#endif