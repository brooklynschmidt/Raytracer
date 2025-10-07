#include <rayutils.h>

Color ray_color(const Ray& r, const int rayCount, const hittable& world) {
    if (rayCount <= 0) {
        return Color(0, 0, 0); // no light contribution when we have no more rays!
    }
    
    hit_record rec;
    /* Susceptible to floating point rounding errors
    0.001 is chosen to ignore hits very close to the calculated intersection point 
    Gets rid of shadow acne
    */
    if (world.hit(r, Interval(0.001, infinity), rec)) {
        /* We add a random unit vector to the surface normal to bias the direction towards the normal
        This produces an effect similar to Lambertian distribution with cosine weights
         */
        Vec3 direction = rec.normal + random_unit_vector();
        return 0.5 * ray_color(Ray(rec.p, direction), rayCount - 1, world);
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