#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"
#include "utils.hpp"

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

#define WHITE Vec3(1, 1, 1)
#define WHITEGRAY Vec3(0.9, 0.9, 0.9)
#define GRAY Vec3(0.5, 0.5, 0.5)
#define SILVER Vec3(0.75, 0.75, 0.75)
#define BLACK Vec3()
#define BLUE Vec3(0, 0.75, 1)
#define PURPLE Vec3(0.5, 0, 1.0)


vector<Sphere> SetUpScene() {
    ImageTexture *earth = new ImageTexture("earth.jpeg");
    ImageTexture *mars = new ImageTexture("mars.jpeg");
    ImageTexture *moon = new ImageTexture("moon.jpeg");
    vector<Sphere> spheres = {
        Sphere(Vec3(50, -1e12, 0), 1e12, GRAY, new Lambertian(new ConstantTexture(GRAY))),
        Sphere(Vec3(50, 1e12 + 25, 0), 1e12, WHITEGRAY, NULL),
        Sphere(Vec3(-1e12 - 0.5, 0, 0), 1e12, PURPLE, new Metal(Vec3(1, 1, 1), 0.8)),
        Sphere(Vec3(1e12 + 100, 0, 0), 1e12, PURPLE, new Metal(Vec3(1, 1, 1), 0.8)),
        Sphere(Vec3(0, 0, -1e12 - 50), 1e12, PURPLE, new Metal(Vec3(1, 1, 1), 0.8)),
        Sphere(Vec3(0, 0, 1e12 + 50), 1e12, PURPLE, new Metal(Vec3(1, 1, 1), 0.8)), 
        Sphere(Vec3(-6.5 + 50, 6, -2), 6, SILVER, new Metal(Vec3(1, 1, 1), 0.05)),
        Sphere(Vec3(4.5 + 50, 4, -2), 4, BLUE, new Lambertian(new ConstantTexture(BLUE))),
        Sphere(Vec3(50, 3, 5), 3, WHITE, new Dielectric(1.5)),
        Sphere(Vec3(50, 2, 11), 2, WHITE, new Lambertian(earth)),
        Sphere(Vec3(-6 + 50, 2, 7), 2, WHITE, new Lambertian(mars)),
        Sphere(Vec3(6 + 50, 2, 7), 2, WHITE, new Lambertian(moon)),
    };

    for (int i = 0; i < 50; i++) {
        double x = RandDouble();
        if (x < 0.5) x = RandDouble(0, 43);
        else x = RandDouble(56, 100);
        double z = RandDouble();
        if (z < 0.5) z = RandDouble(-20, -3);
        else z = RandDouble(12, 20);
        double r = RandDouble(0.2, 1);
        Vec3 location(x, r, z);
        Vec3 color(RandDouble(), RandDouble(), RandDouble());
        double rand = RandDouble();
        if (rand < 0.6) {
            spheres.push_back(Sphere(location, r, color, new Lambertian(new ConstantTexture(color))));
        } else if (rand < 0.8) {
            spheres.push_back(Sphere(location, r, color, new Metal(Vec3(1, 1, 1), 1)));
        } else {
            spheres.push_back(Sphere(location, r, color, new Dielectric(1.5)));
        }
    }
    return spheres;
};


Vec3 RayColor(const Ray &ray, const vector<Sphere> &spheres, int depth) {
    if (depth <= 0) return Vec3();

    bool isHit = false;
    double t = INFINITY;
    Sphere s;
    HitRecord r;
    for (Sphere cur : spheres) {
        HitRecord rec;
        bool hit = cur.hit(ray, rec, 0.001, t);
        if (rec.t > 0.0 && hit) {
            isHit = true;
            if (rec.t < t) {
                t = rec.t;
                s = cur;
                r = rec;
            }
        }
    }

    if (isHit) {
        Ray scattered;
        Vec3 attenuation;
        if (s.material == NULL) return s.color;
        else {
            if (s.material->scatter(ray, r, scattered, attenuation)) {
                return attenuation * s.color * RayColor(scattered, spheres, depth - 1);
            } else {
                return Vec3();
            }
        }
    }

    Vec3 unit = normalize(ray.dir);
    t = 0.5 * ((unit.y) + 1.0);
    return t * Vec3(0, 0, 0);
};


int main() {
    const int width = 1280;
    const int height = 640;
    const int spp = 3000;
    const int maxdepth = 50;
    const double aspectratio = width / height;

    Vec3 eyept(50, 8, 25);
    Vec3 lookat(50, 8, -1);
    Vec3 up(0, 1, 0);
    Camera cam(eyept, lookat, up, 90, aspectratio, 0.1, 10);

    Vec3 *output = new Vec3[width * height]();

    const vector<Sphere> spheres = SetUpScene();

    for (int j = 0; j < height; j++) {
        fprintf(stderr, "\rScanlines progress: %5.2f%%", 100. * j / (height - 1));
        for (int i = 0; i < width; i++) {
            Vec3 color(0, 0, 0);

            for (int s = 0; s < spp; s++) {
                const double u = (i + RandDouble()) / width;
                const double v = (j + RandDouble()) / height;
                Ray r = cam.get_ray(u, v);
                color += RayColor(r, spheres, maxdepth);
            }
            color = color / spp;
            color = Vec3(sqrt(color.x), sqrt(color.y), sqrt(color.z));
            output[(height - j - 1) * width + i] = color;
        }
    }
    FILE *f = fopen("output.ppm", "w");
    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int i = 0; i < width * height; i++) {
        fprintf(f, "%d %d %d ", (int)(255.999 * output[i].x),
                (int)(255.999 * output[i].y), (int)(255.999 * output[i].z));
    }
    std::cout << "\nDone." << std::endl;

    delete[] output;
    return 0;
};