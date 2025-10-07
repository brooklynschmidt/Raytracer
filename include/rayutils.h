#ifndef RAYUTILS_H
#define RAYUTILS_H

#include <raytracer.h>
#include <hittable.h>
#include <string>
#include <mathutils.h>
#include <interval.h>

Color ray_color(const Ray& r, const int rayCount, const hittable& world);

void write_color(std::ostream& out, const Color& pixel_color);

double hit_sphere(const Point& center, double radius, const Ray& r);

#endif