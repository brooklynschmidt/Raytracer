#ifndef QUAD_H
#define QUAD_H

#include <hittable.h>
#include <hittable_list.h>

class Quad : public hittable {
    public:
        Quad(const Point& Q, const Vec3& u, const Vec3& v, shared_ptr<Material> mat) : Q(Q), u(u), v(v), mat(mat) {
            auto n = cross(u, v);
            normal = unit(n);
            Vec3 Q_as_Vec = Vec3(Q.x(), Q.y(), Q.z());
            D = dot(normal, Q_as_Vec);
            w = n / dot(n, n);
            set_bounding_box();
        }

        virtual void set_bounding_box() {
            // Compute bounding box of all 4 vertices
            auto bbox_diagonal1 = AxisAlignedBoundingBox(Q, Q + u + v);
            auto bbox_diagonal2 = AxisAlignedBoundingBox(Q + u, Q + v);
            bbox = AxisAlignedBoundingBox(bbox_diagonal1, bbox_diagonal2);
        }

        AxisAlignedBoundingBox bounding_box() const override { return bbox; }

        bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());

            // No hit if ray is parallel to the plane
            if (std::fabs(denom) < 1e-8) {
                return false;
            }

            // Return false if the hit point parameter t is outside the ray Interval
            Point origin = r.origin();
            Vec3 origin_as_vec = Vec3(origin.x(), origin.y(), origin.z());
            auto t = (D - dot(normal, origin_as_vec)) / denom;
            if (!ray_t.contains(t)) {
                return false;
            }

            // Determine if the hit point lies within the planar shape using its plane coordinates
            auto intersection = r.at(t);
            Vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec)) {
                return false;
            }

            // Ray hits the 2D shape
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;

        }

        virtual bool is_interior(double a, double b, hit_record& rec) const {
            Interval unit_interval = Interval(0, 1);
            // Given the hit point in plane coords, return false if outside; otherwise, set the hit record UV coords and return true
            if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    private:
        Point Q;
        Vec3 u, v;
        shared_ptr<Material> mat;
        AxisAlignedBoundingBox bbox;
        Vec3 normal;
        double D;
        Vec3 w;
};

inline shared_ptr<hittable_list> box(const Point& a, const Point& b, shared_ptr<Material> mat) {
    // Returns 6 sided 3D box that contains opposite vertices a and b
    auto sides = make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates
    auto min = Point(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = Point(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = Vec3(max.x() - min.x(), 0, 0);
    auto dy = Vec3(0, max.y() - min.y(), 0);
    auto dz = Vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<Quad>(Point(min.x(), min.y(), max.z()), dx, dy, mat)); // front
    sides->add(make_shared<Quad>(Point(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
    sides->add(make_shared<Quad>(Point(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
    sides->add(make_shared<Quad>(Point(min.x(), min.y(), min.z()), dz, dy, mat)); // left
    sides->add(make_shared<Quad>(Point(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
    sides->add(make_shared<Quad>(Point(min.x(), min.y(), min.z()), dx, dz, mat));

    return sides;
}

#endif