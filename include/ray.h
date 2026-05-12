#ifndef RAY_H
#define RAY_H

#include <raytracer.h>

class Ray {
    public:
        Ray() {}
        Ray(const Point& origin, const Vec3& direction) : orig(origin), dir(direction) {}
        Ray(const Point& origin, const Vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}

        const Point& origin() const { return orig; }
        const Vec3& direction() const    { return dir; }
        const double& getTime() const { return tm; }  // Time -> motion blur, we send a single ray at some random point in time
        // Will capture the object location at that instant, and we can calculate the light at that instant

        inline Point at(double t) const { 
            return orig + t*dir;
        }

    private:
        Point orig;
        Vec3 dir;
        double tm;
};

#endif