#ifndef SPHERE_H
#define SPHERE_H

#include <hittable.h>
#include <mathutils.h>
#include <interval.h>

class Sphere : public hittable {
  public:
    Sphere(const Point& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override;

  private:
    Point center;
    double radius;
};

#endif