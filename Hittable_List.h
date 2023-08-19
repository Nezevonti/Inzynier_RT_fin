#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "Hittable.h"

class Hittable_List : public Hittable
{
    public:
    Hittable_List() {}
    Hittable_List(Hittable** l, int n) { list = l; list_size = n; }

    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual void setAABB();

    Hittable** list;
    int list_size;
};

#endif //HiTTABLELIST_H