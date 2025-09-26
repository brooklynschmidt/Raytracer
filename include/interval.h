#ifndef INTERVAL_H
#define INTERVAL_H

#include <raytracer.h>
#include <mathutils.h>

class Interval {
  public:
    double min, max;

    Interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    inline double size() const {
        return max - min;
    }

    inline bool contains(double x) const {
        return min <= x && x <= max;
    }

    inline bool surrounds(double x) const {
        return min < x && x < max;
    }

    inline double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const Interval empty, universe;
};

#endif