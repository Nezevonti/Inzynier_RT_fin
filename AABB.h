#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "ray.h"
//#include "Hittable.h"

class Material;
class Hittable;

struct hit_record
{
    float t;
    float u,v;
    Vec3 p;
    Vec3 normal;
    Material* mat_ptr;

    Hittable* hit_ptr;
};


class AABB {
public:
    Vec3 minPoint;
    Vec3 maxPoint;

    AABB();
    AABB(const Vec3& minPoint, const Vec3& maxPoint);


    Vec3 calculateIntersectionPoint(const Ray& ray, float t) const;
    bool intersect(const Ray& r, float t_min, float t_max, hit_record& rec) const;

    bool isPointInside(const Vec3& point);
    void padBB();
};

#endif  // AABB_H