#include <mathutils.h>

QuadraticResult quadratic(const Point& center, double radius, const Ray& r) {
    Vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto h = dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;

    auto discriminant = h*h - a*c;
    return {a, h, c, discriminant};
}
