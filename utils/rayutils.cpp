#include <rayutils.h>
#include <material.h>

Color ray_color(const Ray& r, const int rayCount, const hittable& world, Color background) {
    if (rayCount <= 0) {
        return Color(0, 0, 0); // no light contribution when we have no more rays!
    }
    
    hit_record rec;

    if (!world.hit(r, Interval(0.001, infinity), rec)) {
        return background;
    }

    Ray scattered;
    Color attenuation;
    double pdf_value;
    Color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat->scatter(r, rec, attenuation, scattered, pdf_value)) {
        return color_from_emission;
    }

    double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);
    pdf_value = scattering_pdf;

    Color color_from_scatter = (attenuation * scattering_pdf * ray_color(scattered, rayCount - 1, world, background)) / pdf_value;

    return color_from_emission + color_from_scatter;
}

/* This function transforms our data into gamma space from linear space.
Most image viewers expect gamma space images, but we are providing linear space data.
*/
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        /* Gamma -> Linear is Gamma^2
        The inverse is 1/gamma, or the square root
        */
        return std::sqrt(linear_component);
    }

    return 0;
}

void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transformation
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));
     
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