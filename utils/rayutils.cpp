#include <ray.h>
#include <typed_vec3.h>
#include <rayutils.h>

Color ray_color(const Ray& r) {
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