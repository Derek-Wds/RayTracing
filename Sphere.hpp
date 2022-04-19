#ifndef SPHERE_HEADER_H
#define SPHERE_HEADER_H

#include "HitRecord.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

#include <iostream>

class Sphere {
    public:
        Vec3 center;
        double radius;
        Vec3 color;
        Material *material;
        Sphere() {}
        Sphere(Vec3 c, double r, Vec3 col, Material *m) : center(c), radius(r), color(col), material(m) {}
        bool hit(const Ray &ray, HitRecord &rec, double tmin = -0.001, double tmax = 0.001) const {
            Vec3 oc = ray.orig - center;
            double a = ray.dir.length_squared();
            double b = dot(oc, ray.dir);
            double c = oc.length_squared() - radius * radius;
            double discriminant = b * b - a * c;
            if (discriminant < 0) {
                return false;
            } else {
                double temp = (-b - sqrt(discriminant)) / a;
                if (temp > tmax || temp < tmin) {
                    temp = (-b + sqrt(discriminant)) / a;
                }
                if (temp > tmax || temp < tmin) {
                    return false;
                } else {
                    rec.t = temp;
                    rec.p = ray.at(rec.t);
                    Vec3 out_N = (rec.p - center) / radius;
                    rec.front = dot(ray.dir, out_N) < 0;
                    rec.normal = rec.front ? out_N : -out_N;
                    GetSphereUV((rec.p - center) / radius, rec.u, rec.v);
                    return true;
                }
            }
        }
};

#endif