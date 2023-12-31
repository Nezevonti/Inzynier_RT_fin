#ifndef SPHERE_H
#define SPHERE_H


#include "Hittable.h"
//#include "Material.h"


class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Vec3 cen, float r, Material* m) : center(cen), radius(r), Hittable(m) { setAABB(); };

    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual void setAABB();

    virtual bool isPointInside(Vec3 point);

    Vec3 center;
    float radius;
    //Material* mat_ptr;
};


#endif
