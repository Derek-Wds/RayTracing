#ifndef MATERIAL_HEADER_H
#define MATERIAL_HEADER_H

#include "HitRecord.hpp"
#include "Ray.hpp"
#include "Texture.hpp"
#include "Vec3.hpp"
#include "utils.hpp"

#include <cmath>

class Material {
    public:
        virtual ~Material() = default;
        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Ray &scattered, Vec3 &attenuation) const = 0;
};


class Dielectric final : public Material {
    public:
        double ref_idx;
        Dielectric(double ri) : ref_idx(ri) {}

        bool scatter(const Ray &r_in, const HitRecord &rec, Ray &scattered, Vec3 &attenuation) const override {

            attenuation = Vec3(1.0, 1.0, 1.0);

            const double eta = rec.front ? (1.0 / ref_idx) : ref_idx;

            const Vec3 unit_dir = normalize(r_in.dir);
            const double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
            const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            if (eta * sin_theta > 1.0) {
                const Vec3 reflected = unit_dir - 2 * dot(unit_dir, rec.normal) * rec.normal;
                scattered = Ray(rec.p, reflected);

                return true;
            }

            double r0 = (1 - eta) / (1 + eta);
            r0 *= r0;
            const double reflect_prob = r0 + (1 - r0) * pow(1 - cos_theta, 5);
            if (RandDouble() < reflect_prob) {
                const Vec3 reflected = unit_dir - 2 * dot(unit_dir, rec.normal) * rec.normal;
                scattered = Ray(rec.p, reflected);

                return true;
            }

            Vec3 refracted_parallel = eta * (unit_dir + cos_theta * rec.normal);
            Vec3 refracted_perp =
                -sqrt(1.0 - refracted_parallel.length_squared()) * rec.normal;
            Vec3 refracted = refracted_parallel + refracted_perp;
            scattered = Ray(rec.p, refracted);

            return true;
        }
};


class Lambertian final : public Material {
    public:
        Texture *albedo;
        Lambertian(Texture *t) : albedo(t) {}

        bool scatter(const Ray &r_in, const HitRecord &rec, Ray &scattered, Vec3 &attenuation) const override {

            const Vec3 target = rec.p + rec.normal + RandUnitVec();
            scattered = Ray(rec.p, target - rec.p);
            attenuation = albedo->value(rec.u, rec.v, rec.p);

            return true;
        }
};


class Metal final : public Material {
    public:
        double fuzz;
        Vec3 albedo;
        Metal(const Vec3 &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        bool scatter(const Ray &r_in, const HitRecord &rec, Ray &scattered, Vec3 &attenuation) const override {
            const Vec3 unit_dir = normalize(r_in.dir);
            const Vec3 reflected = unit_dir - 2 * dot(unit_dir, rec.normal) * rec.normal;
            scattered = Ray(rec.p, reflected + fuzz * RandVecInUnitSphere());
            attenuation = albedo;

            return dot(scattered.dir, rec.normal) > 0;
        }
};

#endif