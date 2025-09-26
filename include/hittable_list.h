#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <hittable.h>
#include <vector>
#include <interval.h>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) {add(object);}

        inline void clear() {objects.clear();}

        inline void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        bool hit(const Ray&r, Interval ray_t, hit_record& rec) const override;
    };

#endif
