#ifndef RAYUTILS_H
#define RAYUTILS_H

#include <typed_vec3.h>
#include <ray.h>

Color ray_color(const Ray& r);

void write_color(std::ostream& out, const Color& pixel_color);

#endif