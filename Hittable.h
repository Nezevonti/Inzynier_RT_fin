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
    Material* mat_ptr;

    Hittable() {};
    Hittable(Material* material_ptr) : mat_ptr(material_ptr) {};

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual void setAABB() = 0;

    virtual bool isPointInside(Vec3 point) = 0;
};

#endif