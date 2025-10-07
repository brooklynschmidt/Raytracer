#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <raytracer.h>
#include <cstdlib>

struct QuadraticResult {
    double a;
    double h;
    double c;
    double discriminant;
};

const double infinity = std::numeric_limits<double>::infinity();
const double pi = glm::pi<float>();

QuadraticResult quadratic(const Point& center, double radius, const Ray& r);

inline double degrees_to_radians(double degrees) {
    return glm::radians(degrees);
}

#endif
