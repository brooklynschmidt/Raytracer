#ifndef HITTABLE_H
#define HITTABLE_H

#include <raytracer.h>
#include <interval.h>
#include "aabb.h"

class Material;

class hit_record {
  public:
    Point p;
    Vec3 normal;
    shared_ptr<Material> mat;
    double t;
    double u;
    double v;
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

    virtual AxisAlignedBoundingBox bounding_box() const = 0;
};

class Translate : public hittable {
  public:
    Translate(shared_ptr<hittable> object, const Vec3& offset) : object(object), offset(offset) {
      bbox = object->bounding_box();
      bbox.x.min += offset.x();
      bbox.x.max += offset.x();
      bbox.y.min += offset.y();
      bbox.y.max += offset.y();
      bbox.z.min += offset.z();
      bbox.z.max += offset.z();
    }

    bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override {
      // Move the ray backwards by the offset
      Ray offset_r(r.origin() - offset, r.direction(), r.getTime());

      // Determine whether an intersection exists along the offset ray (and if so, where)
      if (!object->hit(offset_r, ray_t, rec)) {
        return false;
      }

      rec.p = rec.p + offset;

      return true;
    }

    AxisAlignedBoundingBox bounding_box() const override { return bbox; }

  private:
    shared_ptr<hittable> object;
    Vec3 offset;
    AxisAlignedBoundingBox bbox;
};

class Rotate_Y : public hittable {
  public:
    Rotate_Y(shared_ptr<hittable> object, double angle) : object(object) {
      auto radians = degrees_to_radians(angle);
      sin_theta = std::sin(radians);
      cos_theta = std::cos(radians);
      bbox = object->bounding_box();

      Point min(infinity, infinity, infinity);
      Point max(-infinity, -infinity, -infinity);

      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          for (int k = 0; k < 2; k++) {
            auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
            auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
            auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

            auto newx = cos_theta * x + sin_theta * z;
            auto newz = -sin_theta * x + cos_theta * z;

            Vec3 tester(newx, y, newz);
            float xMin, xMax, yMin, yMax, zMin, zMax;

            for (int c = 0; c < 3; c++) {
              if (c == 0) {
                xMin = std::fmin(min.x(), tester.x());
                xMax = std::fmax(max.x(), tester.x());
              } else if (c == 1) {
                yMin = std::fmin(min.y(), tester.y());
                yMax = std::fmax(max.y(), tester.y());
              } else {
                zMin = std::fmin(min.z(), tester.z());
                zMax = std::fmax(max.z(), tester.z());
              }
            }
            Point tmpMin(xMin, yMin, zMin);
            Point tmpMax(xMax, yMax, zMax);
            min = tmpMin;
            max = tmpMax;
          }
        }
      }

      bbox = AxisAlignedBoundingBox(min, max);
    }
    
    bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override {
      // World Space -> Object space
      auto origin = Point((cos_theta * r.origin().x()) - (sin_theta * r.origin().z()), r.origin().y(), (sin_theta * r.origin().x()) + (cos_theta * r.origin().z()));

      auto direction = Vec3((cos_theta * r.direction().x()) - (sin_theta * r.direction().z()), r.direction().y(), (sin_theta * r.direction().x()) + (cos_theta * r.direction().z()));

      Ray rotated_r(origin, direction, r.getTime());

      // Determine whether an intersection exists in object space (and if so, where)
      if (!object->hit(rotated_r, ray_t, rec)) {
        return false;
      }

      // Object Space -> World Space
      rec.p = Point((cos_theta * rec.p.x()) + (sin_theta * rec.p.z()), rec.p.y(), (-sin_theta * rec.p.x()) + (cos_theta * rec.normal.z()));
      rec.normal = Vec3((cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()), rec.normal.y(), (-sin_theta * rec.normal.z()) + (cos_theta * rec.normal.z()));

      return true;
    }

    AxisAlignedBoundingBox bounding_box() const override { return bbox; }

  private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    AxisAlignedBoundingBox bbox;
};

#endif