#ifndef RAY_H
#define RAY_H

#include <raytracer.h>

class Ray {
    public:
        Ray() {}
        Ray(const Point& origin, const Vec3& direction) : orig(origin), dir(direction) {}

        const Point& origin() const { return orig; }
        const Vec3& direction() const { return dir; }

        inline Point at(double t) const { 
            return orig + t*dir;
        }

    private:
        Point orig;
        Vec3 dir;
};

#endif