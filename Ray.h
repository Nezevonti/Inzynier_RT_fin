#ifndef RAYH
#define RAYH

#include "Vec3.h"

class Ray
{
public:
    Ray() {}
    Ray(const Vec3& org, const Vec3& dir);
    Vec3 at(float t) const;

    Vec3 origin;
    Vec3 direction;
    Vec3 directionInverse;
};

#endif