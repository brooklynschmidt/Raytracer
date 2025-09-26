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

inline double random_double() {
    // returns a random real [0, 1)
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // returns a random real [min, max)
    return min + (max-min)*random_double();
}

#endif
