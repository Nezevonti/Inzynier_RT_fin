// Vec3.h
#ifndef Vec3_H
#define Vec3_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>

//#define PI 3.141592653

class Vec3 {
public:
    Vec3() {}
    Vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    void set(float x, float y, float z);

    inline Vec3& operator+=(const Vec3& v2);
    inline Vec3& operator-=(const Vec3& v2);
    inline Vec3& operator*=(const Vec3& v2);
    inline Vec3& operator/=(const Vec3& v2);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);

    inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline void make_unit_vector();

    Vec3 rotate(const Vec3& axis, float angle) const;


    float e[3];
};


inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator*(float t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator/(Vec3 v, float t) {
    return Vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline Vec3 operator*(const Vec3& v, float t) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline float dot(const Vec3& v1, const Vec3& v2) {
    return v1.e[0] * v2.e[0]
        + v1.e[1] * v2.e[1]
        + v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
        v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
        v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}


#endif // Vec3_H
