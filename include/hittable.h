#ifndef HITTABLE_H
#define HITTABLE_H

#include <raytracer.h>
#include <interval.h>

class Material;

class hit_record {
  public:
    Point p;
    Vec3 normal;
    shared_ptr<Material> mat;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {  // outward_normal has unit length
      front_face = dot(r.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : (outward_normal * -1);
    }
};

class hittable {
  public:
    virtual ~hittable() = default;
    
    virtual bool hit(const Ray& r, Interval ray_t, hit_record& rec) const = 0;
};

#endif