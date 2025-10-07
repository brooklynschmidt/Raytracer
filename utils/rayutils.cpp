#include <rayutils.h>

Color ray_color(const Ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, Interval(0, infinity), rec)) {
        Vec3 direction = random_on_hemisphere(rec.normal);
        return 0.5 * ray_color(Ray(rec.p, direction), world);
    }

    Vec3 unit_direction = unit(r.direction());
    double lerp = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - lerp) * Color(1.0, 1.0, 1.0) + lerp * Color(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    static const Interval intensity(0.000, 0.999);

    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));
     
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

double hit_sphere(const Point& center, double radius, const Ray& r) {
    QuadraticResult result = quadratic(center, radius, r);

    if (result.discriminant < 0) {
        return -1.0;
    } else {
        return (result.h - std::sqrt(result.discriminant)) / result.a;
    }
}