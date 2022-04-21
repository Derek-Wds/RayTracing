#ifndef TEXTURE_HEADER_H
#define TEXTURE_HEADER_H

#include "Vec3.hpp"

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(pop)
#endif

#include <iostream>

using namespace std;

// Base texture class.
class Texture
{
public:
    virtual Vec3 value(double u, double v, const Vec3 &p) const = 0;
};

// Constant texture class.
class ConstantTexture : public Texture
{
public:
    Vec3 color;
    ConstantTexture() {}
    ConstantTexture(Vec3 c) : color(c) {}
    virtual Vec3 value(double u, double v, const Vec3 &p) const override
    {
        return color;
    }
};

// Checker texture class.
class CheckerTexture : public Texture
{
public:
    Texture *odd;
    Texture *even;
    CheckerTexture() {}
    CheckerTexture(Texture *t0, Texture *t1) : even(t0), odd(t1) {}
    virtual Vec3 value(double u, double v, const Vec3 &p) const override
    {
        double sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
};

// Image texture class.
class ImageTexture final : public Texture
{

private:
    unsigned char *data = nullptr;
    int width = 0, height = 0;
    int bytes_per_scanline = 0;

public:
    const static int bytes_per_pixel = 3;

    ~ImageTexture() { delete data; }
    ImageTexture() = default;
    ImageTexture(const char *filename)
    {

        int components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel,
                         components_per_pixel);

        if (!data)
        {
            cerr << "ERROR: Could not load texture image file '"
                 << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    Vec3 value(double u, double v, const Vec3 &p) const override
    {

        if (data == nullptr)
            return Vec3(0, 1, 1);

        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        if (i >= width)
            i = width - 1;
        if (j >= height)
            j = height - 1;

        const double color_scale = 1.0 / 255.0;
        const auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return Vec3(color_scale * pixel[0], color_scale * pixel[1],
                    color_scale * pixel[2]);
    }
};

#endif