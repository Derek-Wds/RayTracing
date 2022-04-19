#ifndef CAMERA_HEADER_H
#define CAMERA_HEADER_H

#include "Ray.hpp"
#include "Vec3.hpp"
#include "utils.hpp"

class Camera {
    private:
        Vec3 origin;
        Vec3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;
        Vec3 u, v, w;
        double radius;

    public:
        Camera(Vec3 eye, Vec3 lookat, Vec3 up, double vfov, double aspect,
           double aperture, double focusdist) {
            origin = eye;
            radius = aperture / 2;

            const double theta = DegtoRad(vfov);
            const double half_height = tan(theta / 2);
            const double half_width = aspect * half_height;

            Vec3 w = normalize(eye - lookat);
            Vec3 u = normalize(cross(up, w));
            Vec3 v = cross(w, u);
            horizontal = 2 * half_width * u * focusdist;
            vertical = 2 * half_height * v * focusdist;
            lower_left_corner = origin - half_width * focusdist * u -
                                half_height * focusdist * v - focusdist * w;
        }

        Ray get_ray(double u, double v) const {
            const Vec3 rd = radius * RandVecInUnitDisk();
            const Vec3 offset = u * rd.x + v * rd.y;
            return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset);
        }
};

#endif