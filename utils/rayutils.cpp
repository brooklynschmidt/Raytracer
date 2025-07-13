#include <ray.h>
#include <typed_vec3.h>
#include <rayutils.h>
#include <string>

Color ray_color(const Ray& r) {
    auto t = hit_sphere(Point(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        Vec3 N = unit(r.at(t) - Vec3(0, 0, 1)); 
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    Vec3 unit_direction = unit(r.direction());
    double lerp = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - lerp) * Color(1.0, 1.0, 1.0) + lerp * Color(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);
     
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

double hit_sphere(const Point& center, double radius, const Ray& r) {
    Vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto h = dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h + std::sqrt(discriminant)) / a;
    }
}