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
};

#endif