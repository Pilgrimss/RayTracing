//
// Created by 王颖 on 19/06/2017.
//

#ifndef RAYTRACING_RAYTRACER_H
#define RAYTRACING_RAYTRACER_H
#define MAX_RAY_DEPTH 6

#include "Sphere.h"
#include <vector>
float mix(const float &a, const float &b, const float &mix)
{
    return b * mix + a * (1 - mix);
}

Vec3f trace(
        const Vec3f &rayorig,
        const Vec3f &raydir,
        const vector<Sphere> &spheres,
        const int &depth)
{

    float tnear = INFINITY;
    const Sphere* sphere = NULL;

    for (unsigned i = 0; i < spheres.size(); ++i) {
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < tnear) {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }

    if (!sphere) return Vec3f(2);
    Vec3f surfaceColor = 0;
    Vec3f phit = rayorig + raydir * tnear;
    Vec3f nhit = phit - sphere->center;
    nhit._Normalize();
    float bias = 1e-4;
    bool inside = false;
    if (raydir._Dot(nhit) > 0) nhit = -nhit, inside = true;
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) {
        float facingratio = -raydir._Dot(nhit);
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);

        Vec3f refldir = raydir - nhit * 2 * raydir._Dot(nhit);
        refldir._Normalize();
        Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1);
        Vec3f refraction = 0;
        if (sphere->transparency) {
            float ior = 1.1, eta = (inside) ? ior : 1 / ior;
            float cosi = -nhit._Dot(raydir);
            float cost = sqrt(1 - eta * eta * (1 - cosi * cosi));
            Vec3f refrdir = raydir * eta + nhit * (eta *  cosi - cost);
            refrdir._Normalize();
            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
        }

        surfaceColor = (reflection * fresneleffect + refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
    }
    else {
        for (unsigned i = 0; i < spheres.size(); ++i) {
            if (spheres[i].emissionColor[0] > 0) {
                Vec3f transmission = 1;
                Vec3f lightDirection = spheres[i].center - phit;
                lightDirection._Normalize();
                for (unsigned j = 0; j < spheres.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
                            transmission = 0;
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission *
                                max(float(0), nhit._Dot(lightDirection)) * spheres[i].emissionColor;
            }
        }
    }
    return surfaceColor + sphere->emissionColor;
}

void render(const vector<Sphere> &spheres)
{
    unsigned width = 640, height = 480;
    Vec3f *image = new Vec3f[width * height], *pixel = image;
    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = 45, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x, ++pixel) {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            raydir._Normalize();
            *pixel = trace(Vec3f(0), raydir, spheres, 0);
        }
    }
    ofstream ofs("./raytracer.ppm", ios::out | ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(float(1), image[i][0]) * 255) <<
            (unsigned char)(std::min(float(1), image[i][1]) * 255) <<
            (unsigned char)(std::min(float(1), image[i][2]) * 255);
    }
    ofs.close();
    delete [] image;
}

#endif
