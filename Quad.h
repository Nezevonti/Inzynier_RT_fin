#pragma once
#include "Hittable.h"
class Quad :
    public Hittable
{
public:
    Quad(Vec3 minPoint, Vec3 u, Vec3 v,Material* m);

    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual void setAABB();

    bool insideQuad(float a, float b, hit_record& rec) const;

    Vec3 minPoint; //origin
    Vec3 u;
    Vec3 v;

    Vec3 normal;
    Vec3 w;

    //Material* mat_ptr;
};

