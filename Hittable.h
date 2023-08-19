#ifndef HITABLEH
#define HITABLEH



#include "Ray.h"
#include "AABB.h"

class Material;

/*
struct hit_record
{
    float t;
    float u,v;
    Vec3 p;
    Vec3 normal;
    Material* mat_ptr;
};
*/

class Hittable {
public:

    AABB boundingBox;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual void setAABB() = 0;
};

#endif