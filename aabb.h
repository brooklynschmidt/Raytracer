#ifndef AABB_H
#define AABB_H

#include <interval.h>
#include <raytracer.h>

class AxisAlignedBoundingBox {
    public:
        Interval x, y, z;

        AxisAlignedBoundingBox() {}

        AxisAlignedBoundingBox(const Interval& x, const Interval& y, const Interval& z) :
            x(x), y(y), z(z) {}
        
        AxisAlignedBoundingBox(const Point& a, const Point& b) {
            // the two points are extrema for the bounding box 
            x = (a.x() <= b.x()) ? Interval(a.x(), b.x()) : Interval(b.x(), a.x());
            y = (a.y() <= b.y()) ? Interval(a.y(), b.y()) : Interval(b.y(), a.y());
            z = (a.z() <= b.z()) ? Interval(a.z(), b.z()) : Interval(b.z(), a.z());
        }

        AxisAlignedBoundingBox(const AxisAlignedBoundingBox& box0, const AxisAlignedBoundingBox& box1) {
            x = Interval(box0.x, box1.x);
            y = Interval(box0.y, box1.y);
            z = Interval(box0.z, box1.z);
        }

        const Interval& axis_interval(int n) const {
            if (n == 1) return y;
            if (n == 2) return z;
            return x;
        }

        bool hit(const Ray& r, Interval ray_t) const {
            const Point& ray_orig = r.origin();
            const Vec3& ray_dir = r.direction();

            for (int axis = 0; axis < 3; axis++) {
                const Interval& ax = axis_interval(axis);

                const double adinv_x = 1.0 / ray_dir.x();
                auto t0x = (ax.min - ray_orig.x()) * adinv_x;
                auto t1x = (ax.max - ray_orig.x()) * adinv_x;

                const double adinv_y = 1.0 / ray_dir.y();
                auto t0y = (ax.min - ray_orig.y()) * adinv_y;
                auto t1y = (ax.max - ray_orig.y()) * adinv_y;

                const double adinv_z = 1.0 / ray_dir.z();
                auto t0z = (ax.min - ray_orig.z()) * adinv_z;
                auto t1z = (ax.max - ray_orig.z()) * adinv_z;

                if (axis == 0) {
                    if (t0x < t1x) {
                        if (t0x > ray_t.min) ray_t.min = t0x;
                        if (t1x < ray_t.max) ray_t.max = t1x;
                    } else {
                        if (t1x > ray_t.min) ray_t.min = t1x;
                        if (t0x < ray_t.max) ray_t.max = t0x;
                    }
                    if (ray_t.max <= ray_t.min)
                        return false;
                }

                if (axis == 1) {
                    if (t0y < t1y) {
                        if (t0y > ray_t.min) ray_t.min = t0y;
                        if (t1y < ray_t.max) ray_t.max = t1y;
                    } else {
                        if (t1y > ray_t.min) ray_t.min = t1y;
                        if (t0y < ray_t.max) ray_t.max = t0y;
                    }
                    if (ray_t.max <= ray_t.min)
                        return false;
                }

                if (axis == 2) {
                    if (t0z < t1z) {
                        if (t0z > ray_t.min) ray_t.min = t0z;
                        if (t1z < ray_t.max) ray_t.max = t1z;
                    } else {
                        if (t1z > ray_t.min) ray_t.min = t1z;
                        if (t0z < ray_t.max) ray_t.max = t0z;
                    }
                    if (ray_t.max <= ray_t.min)
                        return false;
                }
            }
            return true;
        }

        int longest_axis() const {
            // returns index of longest axis of the bounding box
            if (x.size() > y.size()) {
                return x.size() > z.size() ? 0 : 2;
            } else {
                return y.size() > z.size() ? 1 : 2;
            }
        }

        static const AxisAlignedBoundingBox empty, universe;

};
#endif