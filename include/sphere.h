#ifndef SPHERE_H
#define SPHERE_H

#include <hittable.h>
#include <mathutils.h>
#include <interval.h>

class Sphere : public hittable {
  public:

    // Stationary Sphere
    Sphere(const Point& static_center, double radius, shared_ptr<Material> mat): center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {
      auto rvec = Vec3(radius, radius, radius);
      bbox = AxisAlignedBoundingBox(static_center - rvec, static_center + rvec);
    }

    // Moving Sphere
    Sphere(const Point& center1, const Point& center2, double radius, shared_ptr<Material> mat):
      center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {
        auto rvec = Vec3(radius, radius, radius);
        AxisAlignedBoundingBox box1(center.at(0) - rvec, center.at(0) + rvec);
        AxisAlignedBoundingBox box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = AxisAlignedBoundingBox(box1, box2);
      }

    bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override;

    AxisAlignedBoundingBox bounding_box() const override { return bbox; }

  private:
    Ray center;
    double radius;
    shared_ptr<Material> mat;
    AxisAlignedBoundingBox bbox;

    static void get_sphere_uv(const Point& p, double& u, double& v) {
      // p: a point on the sphere of radius one, centered at the origin
      // u: returned value [0, 1] of angle around the Y axis from x=-1
      // v: returned value [0, 1] of angle from Y=-1 to X=-1
      auto theta = std::acos(-p.y());
      auto phi = std::atan2(-p.z(), p.x()) + pi;

      u = phi / (2 * pi);
      v = theta / pi;
    }
};

#endif