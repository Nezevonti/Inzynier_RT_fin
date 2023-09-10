#include "Vec3.h"


inline Vec3& Vec3::operator+=(const Vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
    float k = 1.0f / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

Vec3 Vec3::rotate(const Vec3& axis, float angle) const {
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);

    Vec3 crossVector = cross(axis,*this);
    return *this * cosTheta + crossVector * sinTheta + axis * dot(axis, *this) * (1 - cosTheta);
}

void Vec3::set(float x, float y, float z) {
    this->e[0] = x;
    this->e[1] = y;
    this->e[2] = z;
}