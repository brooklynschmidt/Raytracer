#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

class BVH_Node : public hittable {
    public:
        BVH_Node(hittable_list list) : BVH_Node(list.objects, 0, list.objects.size()) {

        }

        BVH_Node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {}

        bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override {
            if (!bbox.hit(r, ray_t)) {
                return false;
            }

            bool hit_left = left->hit(r, ray_t, rec);
            bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        AxisAlignedBoundingBox bounding_box() const override { return bbox; }
    private:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        AxisAlignedBoundingBox bbox;
};

#endif 