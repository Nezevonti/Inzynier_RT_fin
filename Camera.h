#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"
#include "Random.h"

/*
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
*/

inline Vec3 random_in_unit_disk() {
    Vec3 p;
    do {
        p = (Vec3(random_double(), random_double(), 0)*2.0) - Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class Camera {
public:
    
    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist);
    Ray get_ray(float s, float t);

    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    float lens_radius;
};

#endif