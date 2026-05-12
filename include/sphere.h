#ifndef SPHERE_H
#define SPHERE_H

#include <hittable.h>
#include <mathutils.h>
#include <interval.h>

class Sphere : public hittable {
  public:

    // Stationary Sphere
    Sphere(const Point& static_center, double radius, shared_ptr<Material> mat): center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}

    // Moving Sphere
    Sphere(const Point& center1, const Point& center2, double radius, shared_ptr<Material> mat):
      center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override;

  private:
    Ray center;
    double radius;
    shared_ptr<Material> mat;
};

#endif