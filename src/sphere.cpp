#include <sphere.h>

bool Sphere::hit(const Ray& r, Interval ray_t, hit_record& rec) const {
    Point current_center = center.at(r.getTime());
    Vec3 oc = current_center - r.origin();
    QuadraticResult result = quadratic(current_center, radius, r);
    if (result.discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(result.discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (result.h - sqrtd) / result.a;
    if (!ray_t.surrounds(root)) {
        root = (result.h + sqrtd) / result.a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
}