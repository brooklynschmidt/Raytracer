#ifndef RAY_H
#define RAY_H

#include <typed_vec3.h>

class Ray {
    public:
        Ray() {}
        Ray(const Point& origin, const Vec3& direction) : orig(origin), dir(direction) {}

        const Point& origin() const { return orig; }
        const Vec3& direction() const { return dir; }

        Point at(double t) const { 
            return orig + t*dir;
        }

    private:
        Point orig;
        Vec3 dir;
};

#endif