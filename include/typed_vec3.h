#ifndef TYPED_VEC3
#define TYPED_VEC3

#include <glm/glm.hpp>
#include <iostream>
#include <type_traits>

template <typename Tag>

class TypedVec3 {
    public:
        glm::vec3 value;

        TypedVec3() : value(0.0f, 0.0f, 0.0f) {}
        TypedVec3(float x, float y, float z) : value(x, y, z) {}
        explicit TypedVec3(const glm::vec3& v) : value(v) {}

        float x() const { return value.x; }
        float y() const { return value.y; }
        float z() const { return value.z; }

        glm::vec3& val() { return value; }
        const glm::vec3& val() const { return value; }

        TypedVec3& operator-() { return TypedVec3(-value.x, -value.y, -value.z); }

        TypedVec3& operator+=(const TypedVec3& other) {
            value += other.value;
            return *this;
        }

        TypedVec3& operator-=(const TypedVec3& other) {
            value -= other.value;
            return *this;
        }

        TypedVec3& operator*=(float scalar) {
            value *= scalar;
            return *this;
        } 

        TypedVec3& operator/=(float scalar) {
            value /= scalar;
            return *this;
        }

        float magnitude() const {
            return glm::length(value);
        }

};

struct PointTag{};
struct ColorTag{};
struct Vec3Tag{};

using Point = TypedVec3<PointTag>;
using Color = TypedVec3<ColorTag>;
using Vec3 = TypedVec3<Vec3Tag>;

// TypedVec3 Functions

template <typename Tag>
std::ostream& operator<<(std::ostream& out, const TypedVec3<Tag>& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

template <typename Tag>
TypedVec3<Tag> operator+(const TypedVec3<Tag>& a, const TypedVec3<Tag>& b) {
    return TypedVec3<Tag>(a.val() + b.val());
}

template <typename Tag>
TypedVec3<Tag> operator-(const TypedVec3<Tag>& a, const TypedVec3<Tag>& b) {
    return TypedVec3<Tag>(a.val() - b.val());
}

template <typename Tag>
TypedVec3<Tag> operator*(const TypedVec3<Tag>& a, const TypedVec3<Tag>* b) {
    return TypedVec3<Tag>(a.val() * b.val());
}

template <typename Tag>
TypedVec3<Tag> operator*(const TypedVec3<Tag>& a, const float scalar) {
    return TypedVec3<Tag>(a.val() * scalar);
}

template <typename Tag>
TypedVec3<Tag> operator*(const float scalar, const TypedVec3<Tag>& a) {
    return a * scalar;
}

template <typename Tag>
TypedVec3<Tag> operator/(const TypedVec3<Tag>& a, const float scalar) {
    return TypedVec3<Tag>(a.val() / scalar);
}

// Vec3 only functions

inline float dot(const Vec3& a, const Vec3& b) {
    return glm::dot(a.val(), b.val());
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(glm::cross(a.val(), b.val()));
}

inline Vec3 unit(const Vec3& v) {
    return Vec3(glm::normalize(v.val()));
}

// Ray Arithmetic Functions

inline Point operator+(const Point& origin, const Vec3& direction) {
    return Point(origin.val() + direction.val());
}

inline Point operator+(const Vec3& direction, const Point& origin) {
    return origin + direction;
}

inline Point operator-(const Point& origin, const Vec3& direction) {
    return Point(origin.val() - direction.val());
}

inline Point operator-(const Vec3& direction, const Point& origin) {
    return origin - direction;
}

// Subtracting two points doesn't yield another point, but the distance between the two
inline Vec3 operator-(const Point& a, const Point& b) {
    return Vec3(a.val() - b.val());
}

// Convert Point to normalized Vec3
inline Vec3 unit(const Point& p) {
    return Vec3(glm::normalize(p.val()));
}

#endif
